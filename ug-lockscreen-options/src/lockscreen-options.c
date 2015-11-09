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
#include <ui-gadget-module.h>
#include <vconf.h>
#include <vconf-keys.h>
#include <system_settings.h>

#include "lockscreen-options.h"
#include "lockscreen-options-util.h"
#include "lockscreen-options-main.h"

//bundle key
#define SETTING_SUBMENU "setting_submenu"

//for settings search
#define LOCKSCREEN_TYPE "type"
#define LOCKSCREEN_TITLE "title"
#define LOCKSCREEN_UNLOCKEFFECT "Unlock_effect"
#define LOCKSCREEN_CAMERA_SHORTCUT "Camera_shortcut"
#define LOCKSCREEN_HELP_TEXT "Help_text"

static lockscreen_options_ug_data * g_data  = NULL;

static void _launch_layout_locktype_ug_cb(ui_gadget_h ug,
						      enum ug_mode mode,
						      void *priv);
static void _launch_destroy_locktype_ug_cb(ui_gadget_h ug,
						       void *priv);

static Evas_Object *create_bg(Evas_Object * parent)
{
	Evas_Object *bg = elm_bg_add(parent);

	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND,
					 EVAS_HINT_EXPAND);

	elm_object_part_content_set(parent, "elm.swallow.bg", bg);
	evas_object_show(bg);

	return bg;
}

static void _launch_result_password_locktype_ug_cb(ui_gadget_h ug, app_control_h app_control,
				       void *priv)
{
	LOCKOPTIONS_TRACE_BEGIN;
	if(priv == NULL)
		return;
	lockscreen_options_ug_data *ad = (lockscreen_options_ug_data *) priv;	/* ad is point to priv */
	char *result = NULL;
	char *current = NULL;
	app_control_get_extra_data(app_control, "result", &result);
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
		ug_destroy(ug);
		app_control_h svc;
		if (app_control_create(&svc)) {
			return;
		}
		app_control_get_extra_data(app_control, "current", &current);
		if(current){
			app_control_add_extra_data(svc, "current", current);
			free(current);
		}
		////////////////
		//launch_ug("setting-locktype-efl", svc, ad);
		char* ug_name = "setting-locktype-efl";
		lockscreen_options_ug_data *ug_data = (lockscreen_options_ug_data *) ad;
		struct ug_cbs *cbs = (struct ug_cbs *)calloc(1, sizeof(struct ug_cbs));
		if(cbs == NULL){
			app_control_destroy(svc);
			return;
		}
		cbs->layout_cb = _launch_layout_locktype_ug_cb;
		if(strcmp(ug_name, "setting-password-efl") == 0) {
			LOCKOPTIONS_DBG("_launch_result_password_locktype_ug_cb  setting-password-efl");
			cbs->result_cb = _launch_result_password_locktype_ug_cb;
		}  else {
			cbs->result_cb = NULL;
		}
		cbs->destroy_cb = _launch_destroy_locktype_ug_cb;
		cbs->priv = (void *)ug_data;
		LOCKOPTIONS_DBG("Launch ug begin. %s \n", ug_name);
		ui_gadget_h loading = ug_create(ug_data->ug, ug_name, UG_MODE_FULLVIEW, svc, cbs);
		if (NULL == loading) {
			LOCKOPTIONS_ERR("Launch ug failed.");
		}
		///////////////
		app_control_destroy(svc);
		free(cbs);
	}
}
static void _launch_layout_locktype_ug_cb(ui_gadget_h ug,
						      enum ug_mode mode,
						      void *priv)
{
	LOCKOPTIONS_DBG("_launch_layout_locktype_ug_cb begin.\n");
	Evas_Object *base;
	if (!priv)
		return;
	base = (Evas_Object *) ug_get_layout(ug);
	if (!base)
		return;
	switch (mode) {
	case UG_MODE_FULLVIEW:
		evas_object_size_hint_weight_set(base, EVAS_HINT_EXPAND,
						 EVAS_HINT_EXPAND);
		evas_object_show(base);
		break;
	default:
		break;
	}
	LOCKOPTIONS_DBG("_launch_layout_locktype_ug_cb end.\n");
}

static void _launch_destroy_locktype_ug_cb(ui_gadget_h ug, void *priv)
{
	LOCKOPTIONS_TRACE_BEGIN;
	if (!priv)
		return;
	ug_destroy_me(g_data->ug);
	LOCKOPTIONS_TRACE_END;
}

static void _launch_result_locktype_ug_cb(ui_gadget_h ug, app_control_h app_control, void *priv)
{
	LOCKOPTIONS_TRACE_BEGIN;
	if (!priv)
		return;
	ug_destroy_me(g_data->ug);
	LOCKOPTIONS_TRACE_END;
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
		return ECORE_CALLBACK_CANCEL;
	}

	struct ug_cbs *cbs = (struct ug_cbs *)calloc(1, sizeof(struct ug_cbs));
	if(cbs == NULL){
		app_control_destroy(svc_mt_ug);
		return ECORE_CALLBACK_CANCEL;
	}
	cbs->layout_cb = _launch_layout_locktype_ug_cb;
	if(strcmp(ug_name, "setting-password-efl") == 0) {
		cbs->result_cb = _launch_result_password_locktype_ug_cb;
	}  else {
		cbs->result_cb = _launch_result_locktype_ug_cb;
	}
	cbs->destroy_cb = _launch_destroy_locktype_ug_cb;
	cbs->priv = (void *)ug_data;
	LOCKOPTIONS_DBG("Launch ug begin. %s \n", ug_name);
	if(g_data->ug) {
	ui_gadget_h loading = ug_create(g_data->ug, ug_name, UG_MODE_FULLVIEW, svc_mt_ug, cbs);
	if (NULL == loading) {
		LOCKOPTIONS_ERR("Launch ug failed.");
    }
	}
	free(cbs);

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

static Evas_Object *create_frameview(Evas_Object * parent,
				     lockscreen_options_ug_data * ug_data)
{
	Evas_Object *base = NULL;

	/* Create Frame view */

	return base;
}

static void *on_create(ui_gadget_h ug, enum ug_mode mode, app_control_h app_control, void *priv)
{
	LOCKOPTIONS_TRACE_BEGIN;
	Evas_Object *parent = NULL;
	Evas_Object *win_main = NULL;
	lockscreen_options_ug_data *ug_data = NULL;
	int ret = 0;
	char *uri_bundle = NULL;

	if (!ug || !priv)
		return NULL;

	bindtextdomain(PKGNAME, "/usr/ug/res/locale");

	ug_data = priv;
	ug_data->ug = ug;

	parent = ug_get_parent_layout(ug);
	if (!parent)
		return NULL;

	win_main = ug_get_window();
	if (!win_main) {
		return NULL;
	}

	ug_data->win_main = win_main;
	/* Help items */
	ug_data->ly_help = NULL;
	ug_data->popup_help = NULL;
	ug_data->help_end = EINA_FALSE;
	ug_data->uri_bundle = NULL;
	ug_data->help_done_timer = NULL;

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
	/* Help UI <!-- END --> */

	if (mode == UG_MODE_FULLVIEW)
		ug_data->base = create_fullview(parent, ug_data);
	else
		ug_data->base = create_frameview(parent, ug_data);

	/* Add del callback for base layout */

	g_data = ug_data;
	LOCKOPTIONS_TRACE_END;
	return ug_data->base;
}

static void on_start(ui_gadget_h ug, app_control_h app_control, void *priv)
{
}

static void on_pause(ui_gadget_h ug, app_control_h app_control, void *priv)
{
	if(priv == NULL)
		return;
}

static void on_resume(ui_gadget_h ug, app_control_h app_control, void *priv)
{
	LOCKOPTIONS_TRACE_BEGIN;
	LOCKOPTIONS_TRACE_END;
}

static void on_destroy(ui_gadget_h ug, app_control_h app_control, void *priv)
{
	LOCKOPTIONS_TRACE_BEGIN;
	lockscreen_options_ug_data *ug_data;

	_lockscreen_options_unregister_vconf_change();

	if (!ug || !priv)
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

	if(ug_get_layout(ug) != NULL) {

		evas_object_hide((Evas_Object *)ug_get_layout(ug));
		evas_object_del((Evas_Object *)ug_get_layout(ug));
	}

	if(g_data)
		g_data = NULL;
	LOCKOPTIONS_TRACE_END;
}

static void on_message(ui_gadget_h ug, app_control_h msg, app_control_h app_control,
		       void *priv)
{
}

static void on_event(ui_gadget_h ug, enum ug_event event, app_control_h app_control,
		     void *priv)
{
	LOCKOPTIONS_TRACE_BEGIN;
	LOCKOPTIONS_TRACE_END;
}

static void on_key_event(ui_gadget_h ug, enum ug_key_event event,
		app_control_h app_control, void *priv)
{
	if (!ug)
		return;

	switch (event) {
	case UG_KEY_EVENT_END:
		break;
	default:
		break;
	}
}

UG_MODULE_API int UG_MODULE_INIT(struct ug_module_ops *ops)
{
	LOCKOPTIONS_TRACE_BEGIN;
	lockscreen_options_ug_data *ug_data;

	if (!ops)
		return -1;

	ug_data = calloc(1, sizeof(lockscreen_options_ug_data));
	if (!ug_data)
		return -1;

	ops->create = on_create;
	ops->start = on_start;
	ops->pause = on_pause;
	ops->resume = on_resume;
	ops->destroy = on_destroy;
	ops->message = on_message;
	ops->event = on_event;
	ops->key_event = on_key_event;
	ops->priv = ug_data;
	ops->opt = UG_OPT_INDICATOR_ENABLE;
	LOCKOPTIONS_TRACE_END;
	return 0;
}

UG_MODULE_API void UG_MODULE_EXIT(struct ug_module_ops *ops)
{
	lockscreen_options_ug_data *ug_data;

	if (!ops)
		return;

	ug_data = ops->priv;
	if (ug_data) {
		free(ug_data);
		ug_data = NULL;
	}
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


