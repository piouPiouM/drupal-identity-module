<?php

/**
 * @file
 * Admin page callback file for the lr_social_login module.
 */
module_load_include('php', 'lr_social_login', 'LoginRadiusSDK/LoginRadius');
module_load_include('php', 'lr_social_login', 'LoginRadiusSDK/LoginRadiusException');
module_load_include('php', 'lr_social_login', 'LoginRadiusSDK/Clients/IHttpClient');
module_load_include('php', 'lr_social_login', 'LoginRadiusSDK/Clients/DefaultHttpClient');

use \LoginRadiusSDK\LoginRadius;
use \LoginRadiusSDK\LoginRadiusException;

/**
 * Function that build an abstract representation of a HTML form for Social Login settings.
 *
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form.
 * @return array A renderable form array for the respective request.
 */
function lr_social_login_admin_settings($form, &$form_state) {
    if (!variable_get('lr_social_login_apikey', FALSE) || !variable_get('lr_social_login_apisecret', FALSE)) {
        $podata = isset($_POST['op']) ? $_POST['op'] : '';
        if (isset($podata) && $podata != 'Save configuration') {
            drupal_set_message(t('To activate the social login, insert LoginRadius Site Name, API Key and Secret in the social login section below.'), 'warning');
        }
    }

    drupal_add_css(drupal_get_path('module', 'lr_social_login') . '/css/lr_social_login.admin.css');
    drupal_add_js('jQuery(document).ready(function() {
    jQuery("#edit-thanks-block").css({"background-color":"#FFFFE0"});
   });', array('type' => 'inline', 'scope' => 'header', 'weight' => 5));
    $get_lr_social_login = '';

    if (!variable_get('lr_social_login_apikey', FALSE)) {
        $get_lr_social_login = t('For creating a social login account, please visit <a href="@get_lr_social_login" target="_blank">LoginRadius</a>', array('@get_lr_social_login' => 'http://www.Loginradius.com/'));
    }

    $thanks_text = t('To activate the module, you will need to first configure it (manage your desired social networks, etc.) from your LoginRadius account. If you do not have an account, click <a href="@loginradius" target="_blank">here</a>.<br/>We also offer Social Plugins for
  <a href="@wordpress" target="_blank">Wordpress</a>,
  <a href="@drupal" target="_blank">Drupal</a>,
  <a href="@joomla" target="_blank">Joomla</a>,
  <a href="@magento" target="_blank">Magento</a>,
  <a href="@prestashop" target="_blank">Prestashop</a>,
  <a href="@vbulletin" target="_blank">vBulletin</a>,
  <a href="@vanillaforum" target="_blank">VanillaForum</a>,  
  <a href="@phpbb" target="_blank">phpBB</a>,
  <a href="@smf" target="_blank">SMF</a> and
  <a href="@dotnetnuke" target="_blank">DotNetNuke</a>  <br/><a href="@loginradius" target="_blank"><br/><input class="form-submit" type="button" value="Set up my account!"></a> (<a href="@get_sociallogin" target="_blank">How to set up an account?</a>)', array(
      '@loginradius' => 'http://ish.re/4',
      '@wordpress' => 'http://ish.re/10E78',
      '@drupal' => 'http://ish.re/TRXK',
      '@joomla' => 'http://ish.re/12B23',
      '@magento' => 'http://ish.re/UF5L',
      '@prestashop' => 'http://ish.re/TRXU',
      '@vbulletin' => 'http://ish.re/TRXM',
      '@vanillaforum' => 'http://ish.re/TRXR',
      '@dotnetnuke' => 'http://ish.re/TRY1',
      '@phpbb' => 'http://ish.re/TRY3',
      '@smf' => 'http://ish.re/TRY2',
      '@get_sociallogin' => 'http://ish.re/INI1',
    ));
    $form['thanks_block'] = array(
      '#type' => 'fieldset',
      '#title' => t('Thank you for installing the LoginRadius Social Module!'),
      '#description' => $thanks_text,
    );
    $setup_group = 'setup';
    $form[$setup_group] = array(
      '#type' => 'fieldset',
      '#title' => t('LoginRadius API Settings'),
      '#id' => 'setup',
      '#collapsible' => TRUE,
      '#description' => $get_lr_social_login,
    );

    $form[$setup_group]['lr_sso_site_name'] = array(
      '#type' => 'textfield',
      '#title' => t('LoginRadius Site Name'),
      '#required' => TRUE,
      '#default_value' => variable_get('lr_sso_site_name'),
      '#description' => t('You can find the Site Name in your LoginRadius user account'),
    );

    $form[$setup_group]['lr_social_login_apikey'] = array(
      '#type' => 'textfield',
      '#title' => t('API Key'),
      '#required' => TRUE,
      '#default_value' => variable_get('lr_social_login_apikey', ''),
      '#description' => t('To activate the module, enter LoginRadius API Key ( <a href="@get_lr_social_login" target="_blank">How to get it?</a> )', array('@get_lr_social_login' => 'http://ish.re/INI1')),
    );
    $form[$setup_group]['lr_social_login_apisecret'] = array(
      '#type' => 'textfield',
      '#title' => t('API Secret'),
      '#required' => TRUE,
      '#default_value' => variable_get('lr_social_login_apisecret', ''),
      '#description' => t('To activate the module, enter LoginRadius API Secret ( <a href="@get_lr_social_login" target="_blank">How to get it?</a> )', array('@get_lr_social_login' => 'http://ish.re/INI1')),
    );

    $basic_group = 'basic';
    $form[$basic_group] = array(
      '#type' => 'fieldset',
      '#title' => t('SOCIAL LOGIN BASIC SETTINGS'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[$basic_group]['lr_social_login_disable_traditional_login'] = array(
      '#type' => 'radios',
      '#title' => t('Do you want users to log in using social login only?'),
      '#default_value' => variable_get('lr_social_login_disable_traditional_login', 0),
      '#options' => array(
        1 => t('Yes, I want to disable the  Drupal native login form.(In this case, admin can login with his/her linked social account or via /user [example(clean url): site-url/user or example(not clean url): site-url/?q=user])'),
        0 => t('No, I want my users to have both options at their disposal'),
      ),
    );
    $form[$basic_group]['lr_social_login_display_username'] = array(
      '#type' => 'radios',
      '#title' => t('How would you like username to be created? Select the username syntax as per your preference.'),
      '#default_value' => variable_get('lr_social_login_display_username', 0),
      '#options' => array(
        0 => t('Firstname Lastname [Ex: John Doe]'),
        1 => t('Firstname-Lastname [Ex: John-Doe]'),
        2 => t('Email Address [Ex: johndoe@abc.com ]'),
      )
    );
    $form[$basic_group]['lr_social_login_userlogin_redirect'] = array(
      '#type' => 'radios',
      '#title' => t('Redirection settings after login'),
      '#default_value' => variable_get('lr_social_login_userlogin_redirect', 0),
      '#options' => array(
        0 => t('Redirect to same page of site'),
        1 => t('Redirect to profile page of site'),
        2 => t('Redirect to custom page of site (If you want user to be redirected to specific URL after login)'),
      ),
    );
    $form[$basic_group]['lr_social_login_userlogin_redirect']['lr_social_login_custom_redirection'] = array(
      '#type' => 'textfield',
      '#weight' => 50,
      '#default_value' => variable_get('lr_social_login_custom_redirection'),
    );
    $form[$basic_group]['lr_social_login_userregister_redirect'] = array(
      '#type' => 'radios',
      '#title' => t('Redirection settings after registration'),
      '#default_value' => variable_get('lr_social_login_userregister_redirect', 0),
      '#options' => array(
        0 => t('Redirect to same page of site'),
        1 => t('Redirect to profile page of site'),
        2 => t('Redirect to custom page of site (If you want user to be redirected to specific URL after registration)'),
      ),
    );
    $form[$basic_group]['lr_social_login_userregister_redirect']['lr_social_login_custom_register_redirection'] = array(
      '#type' => 'textfield',
      '#weight' => 50,
      '#default_value' => variable_get('lr_social_login_custom_register_redirection', ''),
    );
    $linking_group = 'linking';
    $form[$linking_group] = array(
      '#type' => 'fieldset',
      '#title' => t('SOCIAL ACCOUNT LINKING SETTINGS'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[$linking_group]['lr_social_login_account_linking'] = array(
      '#type' => 'radios',
      '#title' => t('Do you want to enable social account linking at user profile page?'),
      '#default_value' => variable_get('lr_social_login_account_linking', 1),
      '#options' => array(
        1 => t('YES'),
        0 => t('NO'),
      )
    );
    $form[$linking_group]['lr_social_login_identity_label_string'] = array(
      '#type' => 'textfield',
      '#title' => t('What heading do you want to display to the users to link multiple social networks to one account?'),
      '#default_value' => variable_get('lr_social_login_identity_label_string', 'Link to an existing social account'),
      '#description' => t('This text will be displayed just above social login add more identities interface.'),
    );
    // Submit handler.
    $form['#submit'][] = 'lr_social_login_admin_settings_submit';
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
    return $form;
}

function lr_social_login_lr_advance_settings($form, &$form_state) {
    drupal_add_js('jQuery(document).ready(function() {
 var val="' . variable_get("lr_social_login_send_user_credentials") . '";
 if(val == 1){
  jQuery(".form-item-lr-social-login-email-content").show();
 }
 else {
  jQuery(".form-item-lr-social-login-email-content").hide();
 }
  });', array('type' => 'inline', 'scope' => 'header', 'weight' => 4));
    drupal_add_css(drupal_get_path('module', 'lr_social_login') . '/css/lr_social_login.admin.css');
    // Interface settings section.
    $interface_group = 'interface_settings';
    $form[$interface_group] = array(
      '#type' => 'fieldset',
      '#title' => t('Social Login Interface customization'),
      '#id' => 'interface_settings',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[$interface_group]['interface']['lr_social_login_label_string'] = array(
      '#type' => 'textfield',
      '#title' => t('What text do you want to display above the Social Login interface?'),
      '#default_value' => variable_get('lr_social_login_label_string'),
      '#description' => t('Leave empty for no text'),
    );
    $form[$interface_group]['lr_social_login_interface_size'] = array(
      '#type' => 'radios',
      '#title' => t('Select the icon size to use in the Social Login interface'),
      '#default_value' => variable_get('lr_social_login_interface_size', 0),
      '#options' => array(
        0 => t('Small'),
        1 => t('Large'),
      ),
    );
    $form[$interface_group]['lr_social_login_interface_columns'] = array(
      '#type' => 'textfield',
      '#title' => t('How many social icons would you like to be displayed per row?'),
      '#size' => 7,
      '#default_value' => variable_get('lr_social_login_interface_columns', ''),
    );
    $form[$interface_group]['lr_social_login_interface_bgcolor'] = array(
      '#type' => 'textfield',
      '#title' => t('What background color would you like to use for the Social Login interface?'),
      '#default_value' => variable_get('lr_social_login_interface_bgcolor', ''),
      '#description' => t('Leave empty for transparent. You can enter hexa-decimal code as well as name of the color.'),
    );
    // Interface display settings section.
    $display_group = 'interface_display_settings';
    $form[$display_group] = array(
      '#type' => 'fieldset',
      '#title' => t('Social Login Interface Display Settings'),
      '#id' => 'interface_display_settings',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[$display_group]['interface']['lr_social_login_attach_login_form'] = array(
      '#type' => 'radios',
      '#title' => t('Do you want to show the Social Login interface on the user login block?'),
      '#default_value' => variable_get('lr_social_login_attach_login_form', 1),
      '#options' => array(
        1 => t('Yes'),
        0 => t('No'),
      ),
    );
    $form[$display_group]['interface']['lr_social_login_loginpage_position'] = array(
      '#type' => 'radios',
      '#title' => t('Do you want to show the Social Login interface with Drupal`s native login form?'),
      '#default_value' => variable_get('lr_social_login_loginpage_position', 1),
      '#options' => array(
        1 => t('Yes'),
        0 => t('No'),
      ),
    );
    $form[$display_group]['interface']['lr_social_login_registerpage_position'] = array(
      '#type' => 'radios',
      '#title' => t('Do you want to show the Social Login interface with Drupal`s native user registration form?'),
      '#default_value' => variable_get('lr_social_login_registerpage_position', 1),
      '#options' => array(
        1 => t('Yes'),
        0 => t('No'),
      ),
    );
    $form[$display_group]['interface']['lr_social_login_commenting_enable'] = array(
      '#type' => 'radios',
      '#title' => t('Do you want to enable the Social Login interface on the commenting form?'),
      '#default_value' => variable_get('lr_social_login_commenting_enable', 1),
      '#options' => array(
        1 => t('Yes'),
        0 => t('No'),
      )
    );
    $form[$display_group]['interface']['lr_social_login_showicon_location'] = array(
      '#type' => 'radios',
      '#title' => t('How do you want the Social Login interface to be displayed on your Drupal traditional registration/login/commenting form?'),
      '#default_value' => variable_get('lr_social_login_showicon_location', 0),
      '#options' => array(
        0 => t('Above the native registration/login/commenting form'),
        1 => t('Below the native registration/login/commenting form'),
      ),
    );
    // Basic user settings section.
    $user_settings_group = 'user_settings';
    $form[$user_settings_group] = array(
      '#type' => 'fieldset',
      '#title' => t('Social Login User settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[$user_settings_group]['lr_social_login_enable_user_registration'] = array(
      '#type' => 'radios',
      '#title' => t('Do you want to enable Social Login functionality when the Administrators only option is checked?'),
      '#default_value' => variable_get('lr_social_login_enable_user_registration', 0),
      '#options' => array(
        1 => t('Yes'),
        0 => t('No'),
      ),
    );
//    $form[$user_settings_group]['lr_social_login_force_registration'] = array(
//      '#type' => 'radios',
//      '#title' => t('Do you want users to get registered automatically or you want them to manually complete the registration process?'),
//      '#default_value' => variable_get('lr_social_login_force_registration', 1),
//      '#options' => array(
//        1 => t('Automatically register a user and create a new user account'),
//        0 => t('Let the user submit the user registration form after social login process.(Users will be redirected to registration page with user fields auto filled and users finally submit the form to create an account with your website.)'),
//      ),
//    );
    $form[$user_settings_group]['lr_social_login_email_required'] = array(
      '#type' => 'radios',
      '#title' => t('A few network providers do not supply user email address as part of user profile data. Do you want users to provide their email address before completing the registration process?'),
      '#default_value' => variable_get('lr_social_login_email_required', 1),
      '#options' => array(
        1 => t('Yes, get real email address from the users (Ask users to enter their email addresses in a pop-up)'),
        0 => t('No, just auto-generate random email IDs for the users'),
      ),
      '#attributes' => array(
        'onchange' => "showPopupWhenEmailIsEmpty();",
        )
    );
    $form[$user_settings_group]['lr_social_login_emailrequired_popup_top'] = array(
      '#type' => 'textarea',
      '#title' => t('Please enter the title of the pop-up asking users to enter their email address'),
      '#rows' => 2,
      '#default_value' => variable_get('lr_social_login_emailrequired_popup_top', 'Please fill the following details to proceed'),
      '#attributes' => array('placeholder' => t('title'))
    );
    $form[$user_settings_group]['lr_social_login_emailrequired_popup_text'] = array(
      '#type' => 'textarea',
      '#title' => t('Please enter the message to be displayed to the user in the pop-up asking for their email address'),
      '#rows' => 2,
      '#default_value' => variable_get('lr_social_login_emailrequired_popup_text', 'Unfortunately we could not retrieve your email from your @provider account Please enter your email in the form below in order to continue.'),
      '#description' => t('You may use @provider, it will be replaced by the Provider name.'),
    );
    $form[$user_settings_group]['lr_social_login_emailrequired_popup_wrong'] = array(
      '#type' => 'textarea',
      '#title' => t('Please enter the message to be shown to the user in case of an already registered email'),
      '#rows' => 2,
      '#default_value' => variable_get('lr_social_login_emailrequired_popup_wrong', 'This email is already registered. Please enter another email or link this account via account linking on your profile page'),
    );
    $form[$user_settings_group]['lr_social_login_send_user_credentials'] = array(
      '#type' => 'radios',
      '#title' => t('Do you want to send a welcome email to new users after successful registration?'),
      '#default_value' => variable_get('lr_social_login_send_user_credentials', 1),
      '#options' => array(
        1 => t('Yes'),
        0 => t('No'),
      ),
      '#description' => t('Note: It will work only with networks which give email address like Facebook, linkedin.'),
      '#attributes' => array(
        'onclick' => 'if(this.value == 1){
      jQuery(".form-item-lr-social-login-email-content").show();}
      else{
      jQuery(".form-item-lr-social-login-email-content").hide();
      }'
      ),
    );
    $form[$user_settings_group]['lr_social_login_email_content'] = array(
      '#type' => 'textarea',
      '#title' => t('Edit the welcome e-mail messages sent to new member accounts'),
      '#default_value' => lr_social_login_email_content_text(),
      '#rows' => 10,
    );
    $form[$user_settings_group]['lr_social_login_update_profile'] = array(
      '#type' => 'radios',
      '#title' => t('Do you want to update the user profile data in your database everytime a user logs into your website?'),
      '#default_value' => variable_get('lr_social_login_update_profile', 0),
      '#options' => array(
        1 => t('Yes'),
        0 => t('No'),
      )
    );
    module_load_include('inc', 'lr_social_login', 'includes/lr_field_mapping');
    //field mapping section
    $field_map_group = 'fieldmap';
    $form[$field_map_group] = array(
      '#type' => 'fieldset',
      '#title' => t('Social Login Field Mapping'),
      '#description' => t('Each of your <a href="!url">fields that are attached to users</a> are listed below. Map social provider data to field by choosing from select list and configure your user fields from <a href="!url"> FIELD MAPPING </a>section.<br/><br/> For a list of all fields according to the providers:<a href="!data_url" target="_blank"> https://secure.loginradius.com/datapoints </a>', array(
        '!url' => url('admin/config/people/accounts/fields'),
        '!data_url' => url('http://ish.re/7O90')
      )),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[$field_map_group]['lr_social_login_user_fields'] = array(
      '#title' => 'user fields',
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#weight' => 5,
      '#collapsible' => TRUE,
    );
    $properties = lr_field_mapping_user_properties();
    $property_options = array();

    foreach ($properties as $property => $property_info) {
        if (isset($property_info['field_types'])) {
            foreach ($property_info['field_types'] as $field_type) {
                $property_options[$field_type][$property] = $property_info['label'];
            }
        }
    }

    $field_defaults = variable_get('lr_social_login_user_fields', array());
    $instances = field_info_instances('user', 'user');

    foreach ($instances as $field_name => $instance) {
        $field = field_info_field($instance['field_name']);

        if (isset($property_options[$field['type']])) {
            $options = array_merge(array('' => t('- Do not import -')), $property_options[$field['type']]);
            $form[$field_map_group]['lr_social_login_user_fields'][$field_name] = array(
              '#title' => check_plain(t($instance['label'])),
              '#type' => 'select',
              '#options' => $options,
              '#default_value' => isset($field_defaults[$field_name]) ? $field_defaults[$field_name] : '',
            );
        }
        else {
            $form[$field_map_group]['lr_social_login_user_fields'][$field_name] = array(
              '#title' => check_plain(t($instance['label'])),
              '#type' => 'form_element',
              '#children' => '<em>' . t('Not any mappable properties.') . '</em>',
              '#theme_wrappers' => array('form_element'),
            );
        }
    }

    if (module_exists('profile2')) {
        $form[$field_map_group]['lr_social_login_profile_fields'] = array(
          '#title' => 'Profile2 fields',
          '#type' => 'fieldset',
          '#tree' => TRUE,
          '#weight' => 5,
          '#collapsible' => TRUE,
        );

        $properties = lr_field_mapping_user_properties();
        $property_options = array();

        foreach ($properties as $property => $property_info) {
            if (isset($property_info['field_types'])) {
                foreach ($property_info['field_types'] as $field_type) {
                    $property_options[$field_type][$property] = $property_info['label'];
                }
            }
        }

        $field_defaults = variable_get('lr_social_login_profile_fields', array());

        foreach (field_info_instances('profile2') as $field_name => $instances) {
            $form[$field_map_group]['lr_social_login_profile_fields'][$field_name] = array(
              '#type' => 'fieldset',
              '#title' => check_plain(t($field_name)),
              '#tree' => TRUE,
              '#weight' => 5,
              '#collapsible' => TRUE,
            );

            foreach ($instances as $instance => $array) {
                $field = field_info_field($array['field_name']);

                if (isset($property_options[$field['type']])) {
                    $options = array_merge(array('' => t('- Do not import -')), $property_options[$field['type']]);
                    $form[$field_map_group]['lr_social_login_profile_fields'][$field_name][$array['field_name']] = array(
                      '#title' => check_plain(t($array['label'])),
                      '#type' => 'select',
                      '#options' => $options,
                      '#default_value' => isset($field_defaults[$field_name][$array['field_name']]) ? $field_defaults[$field_name][$array['field_name']] : '',
                    );
                }
                else {
                    $form[$field_map_group]['lr_social_login_profile_fields'][$field_name][$array['field_name']] = array(
                      '#title' => check_plain(t($array['label'])),
                      '#type' => 'form_element',
                      '#children' => '<em>' . t('Not any mappable properties.') . '</em>',
                      '#theme_wrappers' => array('form_element'),
                    );
                }
            }
        }
    }

    //debug mode section
    $debug_group = 'debug';
    $form[$debug_group] = array(
      '#type' => 'fieldset',
      '#title' => t('Debug'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[$debug_group]['lr_social_login_debug_mode'] = array(
      '#type' => 'radios',
      '#title' => t('Do you want to enable Debugging mode<a title="Choosing yes will add debug log in database"  style="text-decoration:none"> (<span style="color:#3CF;">?</span>)</a>'),
      '#default_value' => variable_get('lr_social_login_debug_mode', 0),
      '#options' => array(
        1 => t('Yes'),
        0 => t('No'),
      )
    );

    $form['#submit'][] = 'lr_social_login_admin_settings_submit';
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
    return $form;
}

/**
 * Submit Social Login tab settings.
 *
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form
 */
function lr_social_login_admin_settings_submit($form, &$form_state) {
    if (!lr_social_login_save_settings($form_state)) {
        return FALSE;
    }
    $settings = array(
      // 'lr_social_login_label_string' => 'Login with Social ID',
      'lr_social_login_interface_size' => '0',
      'lr_social_login_attach_login_form' => '1',
      'lr_social_login_loginpage_position' => '1',
      'lr_social_login_registerpage_position' => '1',
      'lr_social_login_showicon_location' => '0',
      'lr_social_login_commenting_enable' => '1',
      'lr_social_login_force_registration' => '1',
      'lr_social_login_email_required' => '1',
      'lr_social_login_emailrequired_popup_top' => 'Please fill the following details to proceed',
      'lr_social_login_emailrequired_popup_text' => 'Unfortunately we could not retrieve your email from your @provider account Please enter your email in the form below in order to continue.',
      'lr_social_login_emailrequired_popup_wrong' => 'This email is already registered. Please enter another email or link this account via account linking on your profile page',
      'lr_social_login_send_user_credentials' => '1',
      'lr_social_login_update_profile' => '0',
    );

    foreach ($settings as $key => $values) {
        if (!variable_get($key, FALSE) && variable_get($key, '') != '0') {
            variable_set($key, $values);
        }
    }
}

/**
 * Save settings to database.
 *
 * @param array $form_state A keyed array containing the current state of the form
 */
function lr_social_login_save_settings(&$form_state) {  
   
    $api_key = isset($form_state['values']['lr_social_login_apikey']) ? trim($form_state['values']['lr_social_login_apikey']) : trim(variable_get('lr_social_login_apikey', ''));
    $api_secret = isset($form_state['values']['lr_social_login_apisecret']) ? trim($form_state['values']['lr_social_login_apisecret']) : trim(variable_get('lr_social_login_apisecret', ''));
    $data = lr_social_login_get_authentication($api_key, $api_secret);

    if (isset($form_state['values']['lr_raas_hook_customize'])) {
        $form_state['values']['lr_raas_hook_customize'] = $form_state['input']['lr_raas_hook_customize'];
    }
    if (isset($form_state['values']['lr_raas_hook_valiadtion_message'])) {
    $form_state['values']['lr_raas_hook_valiadtion_message'] = $form_state['input']['lr_raas_hook_valiadtion_message'];
    }
    
    if (isset($data['status']) && $data['status'] != 'status') {
        drupal_set_message($data['message'], $data['status']);
        return FALSE;
    }
    form_state_values_clean($form_state);

    foreach ($form_state['values'] as $key => $value) {
        if (is_array($value) && isset($form_state['values']['array_filter'])) {
            $value = array_keys(array_filter($value));
        }
        variable_set($key, $value);
    }

    drupal_set_message(t('The configuration options have been saved.'));
    // Clear the cached pages and blocks.
    cache_clear_all();
    menu_rebuild();
    return TRUE;
}

/**
 * Function validates against GUID format of keys
 *
 * @param string $key LoginRadius API key/secret
 * @return boolean If valid - true, else - false
 */
function lr_social_login_isvalid_key($key) {
    return preg_match('/^\{?[A-Z0-9]{8}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{12}\}?$/i', $key);
}

/**
 * Check API credentials and API connection methos is working or not
 *
 * @param string $apikey LoginRadius API Key
 * @param string $apisecret LoginRadius API Secret
 * @param string $api_handler API connection method curl/drupal_http_request
 * @return array Contain message and status of data
 */
function lr_social_login_get_authentication($apikey, $apisecret) {
    if (isset($apikey)) {
        $data = array();
        try {
            $validate_url = 'https://api.loginradius.com/api/v2/app/validate?apikey=' . rawurlencode(trim($apikey)) . '&apisecret=' . rawurlencode(trim($apisecret));
            $result = LoginRadius::apiClient($validate_url, FALSE, array('output_format' => 'json'));

            if (empty($result)) {
                $data['message'] = check_plain('please check your php.ini settings to enable CURL or FSOCKOPEN');
                $data['status'] = 'error';
            }
            elseif (isset($result->Status) && !$result->Status) {
                $error = array(
                  "API_KEY_NOT_VALID" => "LoginRadius API key is invalid. Get your LoginRadius API key from <a href='http://www.loginradius.com' target='_blank'>LoginRadius</a>",
                  "API_SECRET_NOT_VALID" => "LoginRadius API Secret is invalid. Get your LoginRadius API Secret from <a href='http://www.loginradius.com' target='_blank'>LoginRadius</a>",
                  "API_KEY_NOT_FORMATED" => "LoginRadius API Key is not formatted correctly",
                  "API_SECRET_NOT_FORMATED" => "LoginRadius API Secret is not formatted correctly"
                );

                foreach ($result->Messages as $value) {
                    $data['message'] = $error["$value"];
                    $data['status'] = 'error';
                    break;
                }
            }

            return $data;
        }
        catch (Exception $e) {
           // watchdog('loginradius_logging', $e, array(), WATCHDOG_ERROR);
        }
    }
}
