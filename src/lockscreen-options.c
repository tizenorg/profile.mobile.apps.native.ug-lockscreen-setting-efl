/*
 * Copyright (c) 2009 - 2015 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */


#ifndef UG_MODULE_API
#define UG_MODULE_API __attribute__ ((visibility("default")))
#endif

#include <Elementary.h>
#include <app.h>
#include <vconf.h>
#include <vconf-keys.h>
#include <system_settings.h>

#include "lockscreen-options.h"
#include "lockscreen-options-util.h"
#include "lockscreen-options-main.h"
#include "lockscreen-options-debug.h"

//bundle key
#define SETTING_SUBMENU "setting_submenu"

//for settings search
#define LOCKSCREEN_TYPE "type"
#define LOCKSCREEN_TITLE "title"
#define LOCKSCREEN_UNLOCKEFFECT "Unlock_effect"
#define LOCKSCREEN_CAMERA_SHORTCUT "Camera_shortcut"
#define LOCKSCREEN_HELP_TEXT "Help_text"

#define FILE_PATH_SIZE 1024
#define LOCALE "locale"

static Evas_Object *create_bg(Evas_Object * parent)
{
	Evas_Object *bg = elm_bg_add(parent);

	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND,
					 EVAS_HINT_EXPAND);

	elm_object_part_content_set(parent, "elm.swallow.bg", bg);
	evas_object_show(bg);

	return bg;
}

static void _launch_result_password_locktype_ug_cb(app_control_h request, app_control_h reply, app_control_result_e app_result, void *priv)
{
	LOCKOPTIONS_TRACE_BEGIN;
	if(priv == NULL)
		return;
	char *result = NULL;
	char *current = NULL;
	app_control_get_extra_data(reply, "result", &result);
	if(result == NULL)
		return;
	LOCKOPTIONS_ERR("_launch_result_password_ug_cb result is %s.", result);

	int ret_int = 0;
	if(strcmp(result, "Cancel") == 0)
	{
		ret_int = 0;
	}
	else if(strcmp(result, "SETTING_PW_TYPE_ENTER_LOCK_TYPE") == 0 || strcmp(result, "SETTING_PW_TYPE_VERIFY_FP_ALT_PASSWORD") == 0)
	{
		ret_int = 1;
	}
	free(result);

	if(ret_int == 1)
	{
		LOCKOPTIONS_DBG("SETTING_PW_TYPE_ENTER_LOCK_TYPE || SETTING_PW_TYPE_VERIFY_FP_ALT_PASSWORD");
		app_control_h svc;
		if (app_control_create(&svc)) {
			return;
		}
		app_control_get_extra_data(reply, "current", &current);
		if(current){
			app_control_add_extra_data(svc, "current", current);
			free(current);
		}
		////////////////
		//launch_ug("setting-locktype-efl", svc, ad);
		char* ug_name = "setting-locktype-efl";

		LOCKOPTIONS_DBG("Launch ug begin. %s \n", ug_name);
	    app_control_set_launch_mode(svc, APP_CONTROL_LAUNCH_MODE_GROUP);
	    app_control_set_app_id(svc, ug_name);
	    if(app_control_send_launch_request(svc, NULL, NULL) != APP_CONTROL_ERROR_NONE)
	    {
			LOCKOPTIONS_ERR("Launch ug failed.");
		}
		///////////////
		app_control_destroy(svc);
	}
}

static Eina_Bool _lockscreen_options_locktype_idler_cb(void *data)
{
	LOCKOPTIONS_TRACE_BEGIN;
	lockscreen_options_ug_data * ug_data = (lockscreen_options_ug_data *)data;
	if(!ug_data)
		return ECORE_CALLBACK_CANCEL;
	//lockscreen_options_locktype_create_view(ug_data);

	int lock_type = 0;
	app_control_h svc_mt_ug = NULL;
	char* ug_name = NULL;
	vconf_get_int(VCONFKEY_SETAPPL_SCREEN_LOCK_TYPE_INT, &lock_type);
	if(!app_control_create(&svc_mt_ug)) {
		if(lock_type == SETTING_SCREEN_LOCK_TYPE_PASSWORD
			|| lock_type == SETTING_SCREEN_LOCK_TYPE_SIMPLE_PASSWORD)
		{
			app_control_add_extra_data(svc_mt_ug, "viewtype", "SETTING_PW_TYPE_ENTER_LOCK_TYPE");
			ug_name = "setting-password-efl";
		}
		else if (lock_type == SETTING_SCREEN_LOCK_TYPE_FINGERPRINT) {
			LOCKOPTIONS_DBG("SETTING_SCREEN_LOCK_TYPE_FINGERPRINT");
			app_control_add_extra_data(svc_mt_ug, "viewtype", "SETTING_PW_TYPE_VERIFY_FP_ALT_PASSWORD");
			ug_name = "setting-password-efl";
		} else {
			ug_name = "setting-locktype-efl";
		}
	}
	if(ug_name == NULL) {
	    app_control_destroy(svc_mt_ug);
		return ECORE_CALLBACK_CANCEL;
	}
    if(svc_mt_ug == NULL) {
        free(ug_name);
        return ECORE_CALLBACK_CANCEL;
    }

    int ret = APP_CONTROL_ERROR_NONE;
    LOCKOPTIONS_DBG("Launch ug begin. %s \n", ug_name);
    app_control_set_launch_mode(svc_mt_ug, APP_CONTROL_LAUNCH_MODE_GROUP);
    app_control_set_app_id(svc_mt_ug, ug_name);

	if(strcmp(ug_name, "setting-password-efl") == 0) {
	    app_control_send_launch_request(svc_mt_ug, _launch_result_password_locktype_ug_cb, ug_data);
	}  else {
	    app_control_send_launch_request(svc_mt_ug, NULL, NULL);
	}

    if(ret != APP_CONTROL_ERROR_NONE)
    {
		LOCKOPTIONS_ERR("Launch ug failed.");
    }

	app_control_destroy(svc_mt_ug);

	LOCKOPTIONS_TRACE_END;

	ug_data->timer_locktype = NULL;
	return ECORE_CALLBACK_CANCEL;
}

static Evas_Object *create_fullview(Evas_Object * parent, lockscreen_options_ug_data * ug_data)
{
	LOCKOPTIONS_TRACE_BEGIN;
	Evas_Object *base = NULL;
	Evas_Object *navi_bar = NULL;

	base = lockscreen_options_util_create_layout(parent, NULL, NULL);

	elm_layout_theme_set(base, "layout", "application", "default");

	elm_win_indicator_mode_set(ug_data->win_main, ELM_WIN_INDICATOR_SHOW);

	create_bg(base);

	navi_bar = lockscreen_options_util_create_navigation(base);
	ug_data->navi_bar = navi_bar;

	//for search settings
	if(ug_data->viewtype!= NULL) {
		if(strcmp(ug_data->viewtype, LOCKSCREEN_TYPE) == 0) {
			LOCKOPTIONS_DBG("_lockscreen_options_locktype_cb");
			if(ug_data->timer_locktype) {
				LOCKOPTIONS_DBG("------------------delete  timer_locktype-------------");
				ecore_timer_del(ug_data->timer_locktype);
				ug_data->timer_locktype = NULL;
			}
			ug_data->timer_locktype = ecore_timer_add(0.0, _lockscreen_options_locktype_idler_cb, ug_data);
			return base;
		}

		if(strcmp(ug_data->viewtype, LOCKSCREEN_UNLOCKEFFECT) == 0) {
			LOCKOPTIONS_DBG("lockscreen_options_main_create_view  unlock effect");
			lockscreen_options_main_create_view(ug_data);
			return base;
		}

		if(strcmp(ug_data->viewtype, LOCKSCREEN_CAMERA_SHORTCUT) == 0) {
			LOCKOPTIONS_DBG("lockscreen_options_main_create_view camera shortcut");
			lockscreen_options_main_create_view(ug_data);
			return base;
		}

		if(strcmp(ug_data->viewtype, LOCKSCREEN_HELP_TEXT) == 0) {
			LOCKOPTIONS_DBG("lockscreen_options_main_create_view help text");
			lockscreen_options_main_create_view(ug_data);
			return base;
		}

	}

	lockscreen_options_main_create_view(ug_data);
	LOCKOPTIONS_TRACE_END;
	return base;
}

static bool on_create(void *priv)
{
	LOCKOPTIONS_TRACE_BEGIN;
	Evas_Object *parent = NULL;
	Evas_Object *win_main = NULL;
	lockscreen_options_ug_data *ug_data = NULL;


	if (!priv)
		return false;

	//TODO: assign correct path to locale
	char *resPath = app_get_resource_path();
	if(resPath)
	{
	    char path[FILE_PATH_SIZE] = { 0, };
	    snprintf(path, sizeof(path), "%s%s", resPath, LOCALE);
	    bindtextdomain(PKGNAME, path);
	    free(resPath);
	}

	ug_data = priv;

	win_main = elm_win_util_standard_add(PKGNAME, PKGNAME);
	if (!win_main) {
		return false;
	}
    elm_win_conformant_set(win_main, EINA_TRUE);
    elm_win_autodel_set(win_main, EINA_TRUE);

    parent = elm_conformant_add(win_main); //check if this necessary
    if (!parent)
        return false;
    evas_object_size_hint_weight_set(parent, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(win_main, parent);
    evas_object_show(parent);
    evas_object_show(win_main);
	ug_data->win_main = win_main;
	/* Help items */
	ug_data->ly_help = NULL;
	ug_data->popup_help = NULL;
	ug_data->help_end = EINA_FALSE;
	ug_data->uri_bundle = NULL;
	ug_data->help_done_timer = NULL;


	/* Help UI <!-- END --> */

	ug_data->base = create_fullview(parent, ug_data);
	elm_object_content_set(parent, ug_data->base);
	/* Add del callback for base layout */
	LOCKOPTIONS_TRACE_END;
	return true;
}

static void on_app_control(app_control_h app_control, void *priv)
{
    LOCKOPTIONS_TRACE_BEGIN;
    lockscreen_options_ug_data *ug_data = NULL;
    int ret = 0;
    char *uri_bundle = NULL;

    LOCKOPTIONS_DBG("priv: %p", priv);
    if (!priv)
        return;
    ug_data = (lockscreen_options_ug_data *)priv;

    app_control_get_extra_data(app_control, SETTING_SUBMENU, &ug_data->extra_data);
    if(ug_data->extra_data != NULL)
        LOCKOPTIONS_DBG("ug_data->extra_data=%s", ug_data->extra_data);

    app_control_get_extra_data(app_control, "viewtype", &ug_data->viewtype);
    if(ug_data->viewtype != NULL)
        LOCKOPTIONS_DBG("viewtype=%s", ug_data->viewtype);

    /* Help UI <!-- START --> */
    ret = app_control_get_uri(app_control, &uri_bundle);
    if(!ret) {
    //if(ret != APP_CONTROL_ERROR_NONE) {
        LOCKOPTIONS_DBG("app_control_get_uri failed, [%d]", ret);
    }

    if(uri_bundle != NULL) {
        if(strncmp(uri_bundle, HELP_UI, strlen(HELP_UI)) == 0) {
            LOCKOPTIONS_DBG("uri_bundle : %s", uri_bundle);
            ug_data->uri_bundle = strdup(uri_bundle);
        } else {
            ug_data->uri_bundle = NULL;
        }
        free(uri_bundle);
    }
    LOCKOPTIONS_TRACE_END;
}

static void on_pause(void *priv)
{
	if(priv == NULL)
		return;
}

static void on_resume(void *priv)
{
	LOCKOPTIONS_TRACE_BEGIN;
	LOCKOPTIONS_TRACE_END;
}

static void on_destroy(void *priv)
{
	LOCKOPTIONS_TRACE_BEGIN;
	lockscreen_options_ug_data *ug_data;

	_lockscreen_options_unregister_vconf_change();

	if (!priv)
		return;
	ug_data = priv;

	if(ug_data->base != NULL) {

		evas_object_del(ug_data->base);
		ug_data->base = NULL;
	}

	if(ug_data->extra_data != NULL) {

		free(ug_data->extra_data);
		ug_data->extra_data = NULL;
	}

	if(ug_data->viewtype!= NULL) {

		free(ug_data->viewtype);
		ug_data->viewtype = NULL;
	}

	if(ug_data->act_pop != NULL) {

		evas_object_del(ug_data->act_pop);
		ug_data->act_pop = NULL;
	}

	/* Help UI */
	if(ug_data->ly_help != NULL) {
		evas_object_del(ug_data->ly_help);
		ug_data->ly_help = NULL;
	}
	if(ug_data->popup_help != NULL) {
		evas_object_del(ug_data->popup_help);
		ug_data->popup_help = NULL;
	}
	if(ug_data->help_done_timer) {
		ecore_timer_del(ug_data->help_done_timer);
		ug_data->help_done_timer = NULL;
	}

	if(ug_data->noti_timer_id) {
		LOCKOPTIONS_DBG("------------------delete  noti_timer-------------");
		ecore_timer_del(ug_data->noti_timer_id);
		ug_data->noti_timer_id = NULL;
	}

	if(ug_data->timer_locktype) {
		LOCKOPTIONS_DBG("------------------delete  timer_locktype-------------");
		ecore_timer_del(ug_data->timer_locktype);
		ug_data->timer_locktype = NULL;
	}

	if(ug_data->timer_destory) {
		LOCKOPTIONS_DBG("------------------delete  timer_destory-------------");
		ecore_timer_del(ug_data->timer_destory);
		ug_data->timer_destory = NULL;
	}

	free(ug_data);
	LOCKOPTIONS_TRACE_END;
}

int main(int argc, char *argv[])
{
    lockscreen_options_ug_data *ug_data;

    ug_data = calloc(1, sizeof(lockscreen_options_ug_data));
    if (!ug_data)
        return -1;

    ui_app_lifecycle_callback_s cbs = {};

    cbs.app_control = on_app_control;
    cbs.create = on_create;
    cbs.pause = on_pause;
    cbs.resume = on_resume;
    cbs.terminate = on_destroy;

    return ui_app_main(argc, argv, &cbs, ug_data);
}

UG_MODULE_API int setting_plugin_reset(app_control_h app_control, void *priv)
{
	LOCKOPTIONS_TRACE_BEGIN;
	int ret = 0;

//for shortcut

	ret += vconf_set_bool(VCONFKEY_LOCKSCREEN_CAMERA_QUICK_ACCESS, 0);

	//for wallpaper
	ret += system_settings_set_value_string(SYSTEM_SETTINGS_KEY_WALLPAPER_LOCK_SCREEN, "/opt/usr/share/settings/Wallpapers/Lock_default.png");
	return ret;
}


