<?php

/**
 * @file
 * Contains \Drupal\sociallogin\Controller\SocialLoginController.
 */
namespace Drupal\sociallogin\Controller;
require_once(drupal_get_path('module', 'sociallogin') . '/guzzleclient.php');

global $apiClient_class;
$apiClient_class = 'LRGuzzleClient';
use Drupal\Core\Controller\ControllerBase;
use LoginRadiusSDK\LoginRadius;
use LoginRadiusSDK\LoginRadiusException;
use LoginRadiusSDK\SocialLogin\SocialLoginAPI;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\user\Entity\User;
use Drupal\Core\Url;
use Symfony\Component\HttpFoundation\RedirectResponse;

/**
 * Returns responses for Social Login module routes.
 */
class SocialLoginController extends ControllerBase {

  protected $user_manager;
  protected $connection;

  public function __construct($user_manager) {
    $this->user_manager = $user_manager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('sociallogin.user_manager')
    );
  }

  /**
   * Response for path 'user/sociallogin'
   *
   * Handle token and validate the user.
   *
   */
  public function userRegisterValidate() {
    $config = \Drupal::config('sociallogin.settings');

    // handle email popup.
    if (isset($_POST['lr_emailclick'])) {
      return $this->user_manager->emailPopupSubmit();
    }
    //clear session of loginradius data when email popup cancel.
    elseif (isset($_POST['lr_emailclick_cancel'])) {
      unset($_SESSION['lrdata']);
      return $this->redirect('<current>');
    }
    elseif (isset($_REQUEST['token'])) {
      $apiSecret = trim($config->get('api_secret'));
      $apiKey = trim($config->get('api_key'));

      try {
        $socialLoginObj = new SocialLoginAPI($apiKey, $apiSecret, array(
          'output_format' => TRUE,
          'authentication' => FALSE
        ));
      }
      catch (LoginRadiusException $e) {
        \Drupal::logger('sociallogin')->error($e);
        drupal_set_message($e->getMessage(), 'error');
        return $this->redirect('user.login');
      }

      //Get Access token.
      try {
        $result_accesstoken = $socialLoginObj->exchangeAccessToken(trim($_REQUEST['token']));
      }
      catch (LoginRadiusException $e) {
        \Drupal::logger('sociallogin')->error($e);
        drupal_set_message($e->getMessage(), 'error');
        return $this->redirect('user.login');
      }

      //Get Userprofile form Access Token.
      try {
        $userprofile = $socialLoginObj->getUserProfiledata($result_accesstoken->access_token);
      }
      catch (LoginRadiusException $e) {
        \Drupal::logger('sociallogin')->error($e);
        drupal_set_message($e->getMessage(), 'error');
        return $this->redirect('user.login');
      }

      if (\Drupal::currentUser()->isAnonymous()) {

        if (isset($userprofile) && isset($userprofile->ID) && $userprofile->ID != '') {
          $userprofile = $this->user_manager->getUserData($userprofile);
          $_SESSION['user_verify'] = 0;

          if ($config->get('email_required') == 1 && empty($userprofile->Email_value)) {
            $uid = $this->user_manager->checkProviderID($userprofile->ID);

            if ($uid) {
              $drupal_user = User::load($uid);
            }

            if (isset($drupal_user) && $drupal_user->id()) {
              return $this->user_manager->provideLogin($drupal_user, $userprofile);
            }
            else {
              $_SESSION['lrdata'] = $userprofile;
              $text_email_popup = $config->get('popup_status');
              $popup_params = array(
                'msg' => $this->t($text_email_popup, array('@provider' => t($userprofile->Provider))),
                'provider' => $userprofile->Provider,
                'msgtype' => 'status',
              );
              $popup_params['message_title'] = $config->get('popup_title');
              return $form['email_popup'] = $this->user_manager->getPopupForm($popup_params);
            }
          }
          return $this->user_manager->checkExistingUser($userprofile);
        }
      }
      else {
        return $this->user_manager->handleAccountLinking($userprofile);
      }
    }
    else {
      //drupal_set_message($this->t('Token is not find'), 'error');
      return $this->redirect('user.login');
    }
  }

  /**
   * Delete Social account of user.
   *
   * @param int $uid
   * @param int $pid
   * @param string $provider
   * @return \Symfony\Component\HttpFoundation\RedirectResponse
   */
  public function userSocialAccountDelete($uid = 0, $pid = 0, $provider = '') {
    $query = $this->user_manager->deleteSocialAccount($pid);
    if ($query) {
      drupal_set_message(t('Your social login identity for %provider successfully deleted.', array('%provider' => $provider)));
    }
    else {
      drupal_set_message(t('We were unable to delete the linked account.'), 'error');
    }
    return $this->redirect('user.login');
  }
}
