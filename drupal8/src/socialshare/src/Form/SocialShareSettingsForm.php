<?php

/**
 * @file
 * Contains \Drupal\captcha\Form\CaptchaSettingsForm.
 */

namespace Drupal\socialshare\Form;

use Drupal\Core\Cache\CacheBackendInterface;
use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\Core\Cache\Cache;

/**
 * Displays the socialshare settings form.
 */
class SocialShareSettingsForm extends ConfigFormBase {

  /**
   * The cache backend.
   *
   * @var \Drupal\Core\Cache\CacheBackendInterface
   */
  protected $cacheBackend;

  /**
   * {@inheritdoc}
   */
  public function __construct(ConfigFactoryInterface $config_factory, CacheBackendInterface $cache_backend) {
    parent::__construct($config_factory);
    $this->cacheBackend = $cache_backend;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static($container->get('config.factory'), $container->get('cache.default'));
  }

  /**
   * {@inheritdoc}
   */
  protected function getEditableConfigNames() {
    return ['socialshare.settings'];
  }

  /**
   * Implements \Drupal\Core\Form\FormInterface::getFormID().
   */
  public function getFormId() {
    return 'socialshare_settings';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $config = $this->config('socialshare.settings');

    module_load_include('inc', 'socialshare');
    global $base_url;
    $my_path = drupal_get_path('module', 'socialshare');
    // Configuration of which forms to protect, with what challenge.
    $form['lr_share_settings'] = [
      '#type' => 'details',
      '#title' => $this->t('Social Sharing Settings'),
      '#open' => TRUE,
      '#attached' => array(
        'library' => array('socialshare/drupal.socialshare_back'),
      ),
    ];

    $form['lr_share_settings']['horizontal'] = [
      '#type' => 'item',
      '#prefix' => '<div>' . t('What social sharing widget theme do you want to use across your website?<div class="description">Horizontal and Vertical themes can be enabled simultaneously</div>') . '</div>',
      '#markup' => ' <div id="lr_tabs"><ul><li><a id="share_horizontal" onclick="display_horizontal_widget();">Horizontal widget</a></li><li><a id="share_veritical" onclick="hidden_horizontal_widget();">Vertical widget</a></li></ul>'
    ];

    $form['lr_share_settings']['interface'] = [
      '#type' => 'hidden',
      '#title' => t('selected share interface'),
      '#default_value' => $config->get('interface'),
      '#suffix' => '<div id=lrsharing_divwhite></div><div id=lrsharing_divgrey></div><div id="show_horizontal_block">',
    ];
    $form['lr_share_settings']['enable_horizontal'] = [
      '#type' => 'radios',
      '#title' => t('Do you want to enable horizontal social sharing for your website?'),
      '#default_value' => $config->get('enable_horizontal'),
      '#options' => [
        1 => $this->t('Yes'),
        0 => $this->t('No'),
      ],
    ];
    $form['lr_share_settings']['enable_vertical'] = [
      '#type' => 'radios',
      '#title' => t('Do you want to enable vertical social sharing for your website?'),
      '#default_value' => $config->get('enable_vertical'),
      '#options' => [
        1 => $this->t('Yes'),
        0 => $this->t('No'),
      ],
    ];


    $form['lr_share_settings']['label'] = [
      '#type' => 'textfield',
      '#title' => $this->t('What text do you want to display above the social sharing widget?'),
      '#default_value' => $config->get('label'),
      '#description' => $this->t('Leave empty for no text'),
    ];
    $form['lr_share_settings']['horizontal_images'] = [
      '#type' => 'radios',
      '#default_value' => $config->get('horizontal_images'),
      '#options' => [
        0 => '<img src="' . $base_url . '/' . $my_path . '/images/horizonSharing32.png"></img>',
        1 => '<img src="' . $base_url . '/' . $my_path . '/images/horizonSharing16.png"></img>',
        10 => '<img src="' . $base_url . '/' . $my_path . '/images/responsive-icons.png"></img>',
        2 => '<img src="' . $base_url . '/' . $my_path . '/images/single-image-theme-large.png"></img>',
        3 => '<img src="' . $base_url . '/' . $my_path . '/images/single-image-theme-small.png"></img>',
        8 => '<img src="' . $base_url . '/' . $my_path . '/images/horizontalvertical.png"></img>',
        9 => '<img src="' . $base_url . '/' . $my_path . '/images/horizontal.png"></img>',
      ],
    ];

    $form['lr_share_settings']['vertical_images'] = [
      '#type' => 'radios',
      '#default_value' => $config->get('vertical_images'),
      '#options' => [
        4 => '<img id= "32VerticlewithBox" src="' . $base_url . '/' . $my_path . '/images/32VerticlewithBox.png"></img>',
        5 => '<img id="VerticlewithBox" src="' . $base_url . '/' . $my_path . '/images/16VerticlewithBox.png"></img>',
        6 => '<img id="hybrid-verticle-vertical" src="' . $base_url . '/' . $my_path . '/images/hybrid-verticle-vertical.png"></img>',
        7 => '<img id="hybrid-verticle-horizontal"  src="' . $base_url . '/' . $my_path . '/images/hybrid-verticle-horizontal.png"></img>',
      ],
    ];
    $form['lr_share_settings']['show_horizotal'] = [
      '#type' => 'hidden',
      '#suffix' => '<div id="share_show_horizontal_widget">',
    ];
    $counter_providers = $config->get('counter_providers');

    if (empty($counter_providers)) {
      $config->set('counter_providers', default_sharing_networks('counter_providers'));

    }
    $form['lr_share_settings']['counter_providers'] = [
      '#type' => 'item',
      '#id' => 'counter_providers',
      '#title' => t('What sharing networks do you want to show in the sharing widget? (All other sharing networks will be shown as part of LoginRadius sharing icon)'),
      '#default_value' => $config->get('counter_providers'),
      '#suffix' => '<div id="socialcounter_hidden_field">',
    ];
    foreach ($config->get('counter_providers') as $provider) {
      if (!empty($provider)) {
        $raw = $provider;
        $provider = str_replace(array(
          ' ',
          '++',
          '+',
        ), array(
          '',
          'plusplus',
          'plus',
        ), $provider);
        $form['lr_share_settings']['counter_rearrange["' . $provider . '"]'] = [
          '#type' => 'hidden',
          '#id' => 'input-lrcounter-' . $provider,
          '#value' => $raw,
          '#attributes' => array('class' => array('lrshare_' . $provider)),
        ];
      }
    }
    $share_providers = $config->get('share_rearrange');

    if (empty($share_providers)) {
      $config->set('share_rearrange', default_sharing_networks('share_rearrange_providers'));
    }
    $form['lr_share_settings']['share_providers'] = [
      '#type' => 'item',
      '#id' => 'share_providers',
      '#title' => t('What sharing networks do you want to show in the sharing widget? (All other sharing networks will be shown as part of LoginRadius sharing icon)'),
      '#default_value' => $config->get('share_rearrange'),
      '#prefix' => '</div><div id="loginRadiusSharingLimit">' . t('You can select only 9 providers.') . '</div>',
      '#suffix' => '<div id="rearrange_sharing_text"><b>' . t('What sharing network order do you prefer for your sharing widget?(Drag around to set the order)') . '</b></div><ul id="share_rearrange_providers" class="share_rearrange_providers">',
    ];
    foreach ($config->get('share_rearrange') as $provider) {
      if (!empty($provider)) {
        $form['lr_share_settings']['share_rearrange[' . $provider . ']'] = array(
          '#type' => 'hidden',
          '#prefix' => '<li id = "edit-lrshare-iconsprite32' . $provider . '" class = "lrshare_iconsprite32 lrshare_' . $provider . '" title = "' . $provider . '">',
          '#default_value' => $provider,
          '#suffix' => '</li>',
        );
      }
    }


    $form['lr_share_settings']['hide_share_rearrange'] = [
      '#type' => 'hidden',
      '#prefix' => '</ul>',
      '#suffix' => '</div>',
    ];
    $form['lr_share_settings']['show_vertical'] = [
      '#type' => 'hidden',
      '#suffix' => '<div id="share_show_veritcal_widget">',
    ];
    $vertical_counter_providers = $config->get('vertical_counter_providers');

    if (empty($vertical_counter_providers)) {
      $config->set('vertical_counter_providers', default_sharing_networks('vertical_counter_rearrange'));
    }
    $form['lr_share_settings']['vertical_counter_providers'] = [
      '#type' => 'item',
      '#id' => 'vertical_counter_providers',
      '#title' => t('What sharing networks do you want to show in the sharing widget? (All other sharing networks will be shown as part of LoginRadius sharing icon)'),
      '#default_value' => $config->get('vertical_counter_providers'),
      '#suffix' => '<div id="socialcounter_vertical_hidden_field" style="display:none;">',
    ];
    foreach ($config->get('vertical_counter_providers') as $provider) {
      if (!empty($provider)) {
        $raw = $provider;
        $provider = str_replace(array(
          ' ',
          '++',
          '+',
        ), array(
          '',
          'plusplus',
          'plus',
        ), $provider);
        $form['lr_share_settings']['vertical_counter_rearrange["' . $provider . '"'] = [
          '#type' => 'hidden',
          '#id' => 'input-lrcounter-vertical-' . $provider,
          '#value' => $raw,
          '#attributes' => array('class' => array('lrshare_vertical_' . $provider)),

        ];
      }
    }

    $vertical_share_providers = $config->get('vertical_share_rearrange');

    if (empty($vertical_share_providers)) {
      $config->set('vertical_share_rearrange', default_sharing_networks('share_rearrange_providers'));

    }

    $form['lr_share_settings']['vertical_share_providers'] = [
      '#type' => 'item',
      '#id' => 'vertical_share_providers',
      '#title' => t('What sharing networks do you want to show in the sharing widget? (All other sharing networks will be shown as part of LoginRadius sharing icon)'),
      '#default_value' => $config->get('vertical_share_rearrange'),
      '#prefix' => '</div><div id="loginRadiusSharingLimit_vertical">' . t('You can select only 9 providers.') . '</div>',
      '#suffix' => '<div id="rearrange_sharing_text_vertical"><b>' . t('What sharing network order do you prefer for your sharing widget?(Drag around to set the order)') . '</b></div><ul id="share_vertical_rearrange_providers" class="share_vertical_rearrange_providers">',
    ];

    foreach ($config->get("vertical_share_rearrange") as $provider) {
      if (!empty($provider)) {
        $form['lr_share_settings']['vertical_share_rearrange[' . $provider . ']'] = array(
          '#type' => 'hidden',
          '#prefix' => '<li id = "edit-lrshare-iconsprite32_vertical' . $provider . '" class = "lrshare_iconsprite32 lrshare_' . $provider . '" title = "' . $provider . '">',
          '#default_value' => $provider,
          '#suffix' => '</li>',
        );
      }
    }
    $form['lr_share_settings']['hide_vertical_share_rearrange'] = [
      '#type' => 'hidden',
      '#prefix' => '</ul>',
      '#suffix' => '</div>',
    ];
    $form['lr_share_settings']['vertical_position'] = [
      '#type' => 'radios',
      '#title' => t('Select the position of social sharing widget'),
      '#default_value' => $config->get('vertical_position'),
      '#options' => [
        0 => t('Top Left'),
        1 => t('Top Right'),
        2 => t('Bottom Left'),
        3 => t('Bottom Right'),
      ],
      '#attributes' => ['style' => 'clear:both'],
    ];
    $form['lr_share_settings']['position_top'] = [
      '#type' => 'checkbox',
      '#title' => t('Show at the top of content.'),
      '#default_value' => $config->get('position_top', 1) ? 1 : 0,
      '#prefix' => '<div id="horizontal_sharing_show"> <b>Select the position of Social sharing interface</b>',
    ];
    $form['lr_share_settings']['position_bottom'] = [
      '#type' => 'checkbox',
      '#title' => t('Show at the bottom of content.'),
      '#default_value' => $config->get('position_bottom', 1) ? 1 : 0,
      '#suffix' => '</div>',
    ];
    $form['lr_share_settings']['show_pages'] = [
      '#type' => 'radios',
      '#title' => t('Show social share on specific pages'),
      '#default_value' => $config->get('show_pages', 0),
      '#options' => [
        0 => t('All pages except those listed'),
        1 => t('Only the listed pages'),
      ],
    ];
    $form['lr_share_settings']['show_exceptpages'] = [
      '#type' => 'textarea',
      '#default_value' => $config->get('show_exceptpages', ''),
      '#description' => t('Enter a page title(you give on page creation) or node id (if url is http://example.com/node/1 then enter 1(node id)) with comma separated'),
      '#rows' => 5,
    ];
    $form['lr_share_settings']['vertical_show_pages'] = [
      '#type' => 'radios',
      '#title' => t('Show social share on specific pages'),
      '#default_value' => $config->get('vertical_show_pages', 0),
      '#options' => [
        0 => t('All pages except those listed'),
        1 => t('Only the listed pages'),
      ],
    ];
    $form['lr_share_settings']['vertical_show_exceptpages'] = [
      '#type' => 'textarea',
      '#default_value' => $config->get('vertical_show_exceptpages', ''),
      '#description' => t('Enter a page title(you give on page creation) or node id (if url is http://example.com/node/1 then enter 1(node id)) with comma separated'),
      '#rows' => 5,
      '#suffix' => '</div>',
    ];
    // Submit button.
    $form['actions'] = ['#type' => 'actions'];
    $form['actions']['submit'] = [
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    ];

    return parent::buildForm($form, $form_state);
  }


  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    //  parent::SubmitForm($form, $form_state);
    $values = $form_state->getValues();
    $inputs = $form_state->getUserInput();
    $sharing_rearrange = array();
    $vertical_share_rearrange = array();
    $this->config('socialshare.settings')

      // Remove unchecked types.
      ->set('enable_horizontal', $values['enable_horizontal'])
      ->set('enable_vertical', $values['enable_vertical'])
      ->set('label', $values['label'])
      ->set('horizontal_images', $values['horizontal_images'])
      ->set('vertical_images', $values['vertical_images'])
      ->set('counter_providers', $values['counter_providers'])
      ->set('counter_rearrange', $values['counter_providers'])
      ->set('share_providers', $inputs['share_rearrange'])
      ->set('share_rearrange', $inputs['share_rearrange'])
      ->set('vertical_counter_rearrange', $values['vertical_counter_providers'])
      ->set('vertical_counter_providers', $values['vertical_counter_providers'])
      ->set('vertical_share_providers', $inputs['vertical_share_rearrange'])
      ->set('vertical_share_rearrange', $inputs['vertical_share_rearrange'])
      ->set('vertical_position', $values['vertical_position'])
      ->set('position_top', $values['position_top'])
      ->set('position_bottom', $values['position_bottom'])
      ->set('show_pages', $values['show_pages'])
      ->set('show_exceptpages', $values['show_exceptpages'])
      ->set('vertical_show_pages', $values['vertical_show_pages'])
      ->set('vertical_show_exceptpages', $values['vertical_show_exceptpages'])
      ->save();

    drupal_set_message(t('Social Share settings have been saved.'), 'status');
    //Clear page cache
    foreach (Cache::getBins() as $service_id => $cache_backend) {
      if ($service_id == 'dynamic_page_cache') {
        $cache_backend->deleteAll();
      }
    }

  }


}
