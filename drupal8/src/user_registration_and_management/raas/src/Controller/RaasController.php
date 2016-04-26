<?php

/**
 * @file
 * Contains \Drupal\raas\Controller\raasController.
 */
namespace Drupal\raas\Controller;

use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\user\Entity\User;
use Drupal\Core\Access\AccessResult;
use \LoginRadiusSDK\LoginRadiusException;
/**
 * Returns responses for Social Login module routes.
 */
class RaasController extends ControllerBase {

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
      $container->get('raas.user_manager')
    );
  }

  /**
   * Response for path 'user/raas'
   *
   * Handle token and validate the user.
   *
   * @todo add destination handling: https://www.drupal.org/node/2524328
   */
  public function userChangePassword($user) {

    $post_value = $_POST;
    $raasid = $this->user_manager->raas_get_raas_uid($user);
    if (isset($post_value['emailid']) && !empty($post_value['emailid']) && isset($post_value['password']) && !empty($post_value['password'])) {
      $params = array(
        'accountid' => $raasid,
        'password' => $post_value['password'],
        'emailid' => $post_value['emailid']
      );

      $result = $this->user_manager->create_raas_profile($params);
    //  $response = $raas_sdk->raasGetRaasProfile($raasid);
    //  lr_social_login_insert_into_mapping_table($response->ID, 'RAAS', $user);
      if (isset($result->isPosted) && $result->isPosted) {
        drupal_set_message(t('Password set successfully.'));
      }
      else {        
        $msg = isset($result) ? $result : 'Password is not set';
        drupal_set_message(t( $msg), 'error');
      }
    }

    elseif (isset($post_value['newpassword']) && !empty($post_value['newpassword'])) {      
      if (!empty($raasid)) {   
        $result = $this->user_manager->update_user_password($raasid, $post_value['oldpassword'], $post_value['newpassword']);
        if (isset($result->isPosted) && $result->isPosted) {        
          drupal_set_message(t('Password changed successfully.'));
        }
        else {    
           $msg = isset($result) ? $result : 'Password is not changed';
          drupal_set_message(t($msg), 'error');
        }
      }
    }
  
    $output = array(
      '#title' => t('Change Password'),
        '#theme' => 'change_password',
         '#attributes' => array('class' => array('change-password'))
      );
    return $output;
  }
  public function changePasswordAccess(){
    $config = \Drupal::config('sociallogin.settings');
    $user = \Drupal::currentUser()->getRoles();
    $access_granted = in_array("administrator", $user);
    $optionVal = $config->get('raas_email_verification_condition');
    if ($access_granted) {
        return AccessResult::forbidden();
    } elseif ($optionVal == 1 || $optionVal == 2) {
        if (strtolower($_SESSION['provider']) == 'raas' || $_SESSION['emailVerified']) {              
           return AccessResult::allowed();
        } else {         
            return AccessResult::forbidden();
        }
    }
   return AccessResult::allowed();  
      
  }
  
}
