jQuery(document).ready(function () {
     showMailchimpUI();
     showPopupWhenEmailIsEmpty();
    var showChar = 200;
    var ellipsestext = "...";
    var moretext = "more";
    var lesstext = "less";
    jQuery('.more').each(function() {
        var thisis = jQuery(this);
        var content = thisis.html(); 
        if(content.length > showChar) { 
            var contnt = content.substr(0, showChar);
            var contntLength = content.substr(showChar-1, content.length - showChar); 
            var html = contnt + '<span class="moreellipses">' + ellipsestext+ '&nbsp;</span><span class="morecontent"><span>' + contntLength + '</span>&nbsp;&nbsp;<a href="" class="morelink">' + moretext + '</a></span>';
             thisis.html(html);
        } 
    });
 
    jQuery(".morelink").click(function(){
        var thisis = jQuery(this);
        if(thisis.hasClass("less")) {
            thisis.removeClass("less");
            thisis.html(moretext);
        } else {
            thisis.addClass("less");
            thisis.html(lesstext);
        }
        thisis.parent().prev().toggle();
        thisis.prev().toggle();
        return false;
    });  
});

function showMailchimpUI() {
    var options = jQuery('input[name=lr_mailchimp_enable]:checked').val();
    if (options == 0) {    
        jQuery('.form-item-lr-mailchimp-apikey,#edit-mailchimp-save,#mailchimp-display-div,#mailchimp-fields-div,#mapping-field-title,.mapping-field-description,.form-item-lr-mailchimp-list').hide();
     } else {
        jQuery('.form-item-lr-mailchimp-apikey,#edit-mailchimp-save,#mailchimp-display-div,#mailchimp-fields-div,#mapping-field-title,.mapping-field-description,.form-item-lr-mailchimp-list').show();
    }
}
function showPopupWhenEmailIsEmpty() {
    var options = jQuery('input[name=lr_social_login_email_required]:checked').val();
    if (options == 0) {    
        jQuery('.form-item-lr-social-login-emailrequired-popup-top,.form-item-lr-social-login-emailrequired-popup-text,.form-item-lr-social-login-emailrequired-popup-wrong').hide();
     } else {
        jQuery('.form-item-lr-social-login-emailrequired-popup-top,.form-item-lr-social-login-emailrequired-popup-text,.form-item-lr-social-login-emailrequired-popup-wrong').show();
    }
}
 


