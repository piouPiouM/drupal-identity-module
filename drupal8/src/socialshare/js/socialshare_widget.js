var script = document.createElement('script');
script.type = 'text/javascript';
script.text = 'var islrsharing = true; var islrsocialcounter = true;';
document.body.appendChild(script);

var script_new = document.createElement('script');
script_new.type = 'text/javascript';
script_new.src = '//share.lrcontent.com/prod/v1/LoginRadius.js';
document.body.appendChild(script_new);

window.onload = function () {
    if (typeof LoginRadius != 'undefined') {
        if (drupalSettings.advancelrsocialshare != undefined) {
            if (drupalSettings.advancelrsocialshare.sharing != undefined && drupalSettings.advancelrsocialshare.sharing) {
                LoginRadius.util.ready(function () {
                    var lr_interface = drupalSettings.advancelrsocialshare.lr_interface;
                    $i = $SS.Interface[lr_interface];
                    var string = drupalSettings.advancelrsocialshare.providers;
                    var providers = string.split(',');
                    $SS.Providers.Top = providers;
                    $u = LoginRadius.user_settings;
                    if (drupalSettings.advancelrsocialshare.apikey) {
                        $u.apikey = drupalSettings.advancelrsocialshare.apikey;
                    }
                    $u.sharecounttype = 'url';
                    $i.size = drupalSettings.advancelrsocialshare.size;
                    $i.top = '0px';
                    $i.left = '0px';
                    if (typeof document.getElementsByName("viewport")[0] != "undefined") {
                        $u.isMobileFriendly = true;
                    }
                    ;
                    $i.show(drupalSettings.advancelrsocialshare.divwidget);
                });
            }
            if (drupalSettings.advancelrsocialshare.counter != undefined && drupalSettings.advancelrsocialshare.counter) {
                LoginRadius.util.ready(function () {
                    $u = LoginRadius.user_settings;
                    var string = drupalSettings.advancelrsocialshare.providers;
                    var providers = string.split(',');
                    var lr_interface = drupalSettings.advancelrsocialshare.lr_interface;
                    $SC.Providers.Selected = providers;
                    $S = $SC.Interface[lr_interface];
                    $S.isHorizontal = true;
                    $S.countertype = drupalSettings.advancelrsocialshare.countertype;
                    if (typeof document.getElementsByName("viewport")[0] != "undefined") {
                        $u.isMobileFriendly = true;
                    }
                    ;
                    $S.show(drupalSettings.advancelrsocialshare.divwidget);
                });
            }
            if (drupalSettings.advancelrsocialshare.verticalsharing != undefined && drupalSettings.advancelrsocialshare.verticalsharing) {
                LoginRadius.util.ready(function () {
                    var lr_interface = drupalSettings.advancelrsocialshare.lr_vertical_interface;
                    $i = $SS.Interface[lr_interface];
                    var string = drupalSettings.advancelrsocialshare.vertical_providers;
                    var providers = string.split(',');
                    $SS.Providers.Top = providers;
                    $u = LoginRadius.user_settings;
                    if (drupalSettings.advancelrsocialshare.vertical_apikey) {
                        $u.apikey = drupalSettings.advancelrsocialshare.vertical_apikey;
                    }
                    $u.sharecounttype = 'url';
                    $i.size = drupalSettings.advancelrsocialshare.vertical_size;
                    if (drupalSettings.advancelrsocialshare.vertical_offset && drupalSettings.advancelrsocialshare.vertical_offset != '0px') {
                        $i.top = drupalSettings.advancelrsocialshare.vertical_offset;
                    }
                    else {
                        $i[drupalSettings.advancelrsocialshare.vertical_position1] = '0px';
                    }
                    $i[drupalSettings.advancelrsocialshare.vertical_position2] = '0px';
                    if (typeof document.getElementsByName("viewport")[0] != "undefined") {
                        $u.isMobileFriendly = true;
                    }
                    ;
                    $i.show(drupalSettings.advancelrsocialshare.vertical_divwidget);
                });
            }
            if (drupalSettings.advancelrsocialshare.verticalcounter != undefined && drupalSettings.advancelrsocialshare.verticalcounter) {
                LoginRadius.util.ready(function () {
                    var string = drupalSettings.advancelrsocialshare.vertical_providers;
                    var providers = string.split(',');
                    $u = LoginRadius.user_settings;
                    $SC.Providers.Selected = providers;
                    $S = $SC.Interface.simple;
                    $S.isHorizontal = false;
                    $S.countertype = drupalSettings.advancelrsocialshare.vertical_countertype;
                    if (drupalSettings.advancelrsocialshare.vertical_offset && drupalSettings.advancelrsocialshare.vertical_offset != '0px') {
                        $S.top = drupalSettings.advancelrsocialshare.vertical_offset;
                    }
                    else {
                        $S[drupalSettings.advancelrsocialshare.vertical_position1] = '0px';
                    }

                    $S[drupalSettings.advancelrsocialshare.vertical_position2] = '0px';
                    if (typeof document.getElementsByName("viewport")[0] != "undefined") {
                        $u.isMobileFriendly = true;
                    }
                    ;
                    $S.show(drupalSettings.advancelrsocialshare.vertical_divwidget);
                });
            }
        }
    }
}