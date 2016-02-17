SOCIAL LOGIN

-- SUMMARY --

Let your users log in and comment via their existing ID from popular providers 
such as Facebook, Google, Twitter, Yahoo, Live and over 20 more!

 -- REQUIREMENTS --
 LoginRadius PHP SDK library. Follow the installation instructions to add require php sdk library.
 -- INSTALLATION --

1. Install as usual, 
see https://www.drupal.org/documentation/install/modules-themes/modules-8 for
further information.
2. After successfully installing, you will see LoginRadius Unified Social API module in modules list in your site's admin account but do NOT enable the module yet because the required LoginRadius PHP SDK library is not installed.
3. Module comes with a file loginradius/sociallogin/composer.json. This file contains the dependency to LoginRadius PHP SDK so that Composer will know to download the SDK library in the next step.
3. Download and initialize Composer Manager to the /modules directory.
4. Let Composer download LoginRadius PHP SDK library for you. On command line of your server:
  1. Go to the root directory of your Drupal installation.
  2. Execute the following command to install php sdk only 
     composer require loginradius/php-sdk-2.0:dev-master
5. After Successfully install LoginRadius PHP SDK, Enable Social Login and Social Share Module.
3. Click on configuration link shown in Social Login and Social Share module or click on 
configuration tab, Then go to people block and click on Social Login and Social Share 
4. On configuration page, you will see config option for Social Login and Social Share module .

 -- LIVE DEMO --
http://demo.loginradius.com

 -- FAQ --

 Q: What is LoginRadius?

 A: LoginRadius is a Software As A Service (SAAS) which allows users to log in 
 to a third party website via 
 popular open IDs/oAuths such as Google, Facebook, Yahoo, AOL and over 20 more.
 
Q: How long can I keep my account?

A: How long you use LoginRadius is completely up to you. You may remove 
LoginRadius 
from your website and delete your account at any time.

Q: What is the best way to reach the LoginRadius Team? 

A: If you have any questions or concerns regarding LoginRadius, 
please write us at hello@loginradius.com.

Q: How much you charge for this service?

A: It is FREE and will remain free, but for advanced features and customized 
solutions, 
there are various packages available. Please contact us for further 
details.

Q: Do you have a live demo site?

A: Yes, please visit our Drupal live demo site at 
http://demo.loginradius.com


 -- CONTACT --

 Current maintainers:
 * LoginRadius - http://www.loginradius.com
 * Email: hello [at] loginradius [dot] com 

 

 
