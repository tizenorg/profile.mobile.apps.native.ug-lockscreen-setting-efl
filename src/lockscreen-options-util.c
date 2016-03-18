/*
 *  starter
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Seungtaek Chung <seungtaek.chung@samsung.com>, Mi-Ju Lee <miju52.lee@samsung.com>, Xi Zhichan <zhichan.xi@samsung.com>
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

#include <vconf.h>
#include <vconf-keys.h>
#include <app_control_internal.h>

#include "lockscreen-options-debug.h"
#include "lockscreen-options.h"
#include "lockscreen-options-util.h"

const char *sys_str_table[] = {
	"IDS_COM_SK_OK",
	"IDS_COM_SK_CANCEL",
	"IDS_COM_BODY_FACEBOOK",
};

const char *app_str_table[] = {
	"IDS_ST_HEADER_LOCK_SCREEN_SETTINGS",
	"IDS_LCKSCN_HEADER_PERSONAL_MESSAGE",
	"IDS_HS_HEADER_THEMES",
	"IDS_LCKSCN_HEADER_LOCK_SCREEN",
	"IDS_LCKSCN_HEADER_SET_SHORTCUTS",
	"IDS_ST_BODY_SCREEN_LOCK_TYPE",
	"IDS_LCKSCN_MBODY_WALLPAPERS",
	"IDS_ST_BODY_SHORTCUTS",
	"IDS_ST_BODY_SET_SHORTCUTS_ON_LOCK_SCREEN",
	"IDS_ST_BODY_SHORTCUTS",
	"IDS_LCKSCN_BODY_CUSTOMISE_WHICH_SHORTCUTS_WILL_BE_SHOWN_ON_THE_LOCK_SCREEN",
	/* stytle start */
	"IDS_LCKSCN_BODY_LOCK_SCREEN_THEMES_ABB",
	"",
	"Set contents on the lock screen",
	"IDS_LCKSCN_BODY_CUSTOMISE_THE_STYLE_OF_YOUR_LOCK_SCREEN",
	/* stytle end */
	"IDS_LCKSCN_BODY_BRIEFING_BOX_ABB",
	"Missed event, Weather, Shortcuts...",
	"IDS_LCKSCN_BODY_CUSTOMISE_WHICH_ITEMS_WILL_BE_SHOWN_IN_YOUR_BRIEFING_BOX_ON_THE_LOCK_SCREEN",
	"IDS_LCKSCN_BODY_PERSONAL_MESSAGE_ABB",
	"IDS_LCKSCN_BODY_DISPLAY_YOUR_PERSONAL_MESSAGE_ON_THE_LOCK_SCREEN",
	//"Event notifications",
	"IDS_IDLE_MBODY_EVENT_NOTIFICATIONS",
	//"View event notification on lock screen",
	"IDS_IDLE_BODY_VIEW_EVENT_NOTIFICATIONS_ON_THE_LOCK_SCREEN",
	"Context-aware notifications",
	"View context-aware notifications on lock screen",
#if USE_CAMERA
	"IDS_ST_MBODY_CAMERA_QUICK_ACCESS",
	"IDS_ST_BODY_TAP_AND_HOLD_THE_SCREEN_WHILE_ROTATING_THE_DEVICE_TO_OPEN_CAMERA",
#endif
	"IDS_ST_BODY_CLOCK",
	"IDS_LCKSCN_MBODY_DUAL_CLOCK",
	"IDS_ST_BODY_SHOW_DUAL_CLOCK_ON_LOCK_SCREEN_WHEN_ROAMING",
	"IDS_ST_MBODY_SET_HOME_CITY",
	"IDS_LCKSCN_BODY_WAKE_UP_IN_LOCK_SCREEN_ABB",
	"IDS_HELP_BODY_UNLOCK_DEVICE_WITH_YOUR_WAKE_UP_COMMAND_WHEN_SWIPE_OR_MOTION_SCREEN_LOCK_TYPE_IS_ENABLED",
	"IDS_SKM_BODY_WEATHER_M_IDEA_SKETCH",
	"IDS_IM_BODY_HELP_TEXT",
	"IDS_ST_BODY_SHOW_HELP_TEXT_ON_LOCK_SCREEN",
	"IDS_ST_BODY_SET_SHORTCUTS",
	"IDS_ST_OPT_EDIT_SHORTCUTS",
	"IDS_ST_BODY_SET_SHORTCUTS_ON_LOCK_SCREEN",
	"IDS_ST_BODY_TAP_SHORTCUT_TO_ASSIGN_APPLICATION",
	"IDS_ST_BODY_DRAG_AND_DROP_TO_CHANGE_ORDER",
	"IDS_ST_BODY_SELECT_APPLICATION",
	"IDS_LCKSCN_BODY_PERSONAL_MESSAGE_ABB",
	"IDS_LCKSCN_HEADER_LOCK_SCREEN_THEMES_ABB",
	"IDS_ST_BODY_SET_AS_LOCK_SCREEN",
	"IDS_ST_BODY_PREVIEW",
	"IDS_ST_BODY_APPLY",
	"IDS_LCKSCN_HEADER_BRIEFING_BOX",
	"IDS_ST_BODY_ITEMS",
	"IDS_LCKSCN_BODY_MUSIC_CONTROLLER_ABB",
	"IDS_IDLE_MBODY_EVENT_NOTIFICATIONS",
	"IDS_LCKSCN_BODY_NEWS_ABB",
	"IDS_LCKSCN_BODY_SCHEDULE",
	"IDS_LCKSCN_BODY_CLEAR_M_LOCKSCREEN_NAME",
	"IDS_LCKSCN_BODY_CLOSE_UP_M_LOCKSCREEN_NAME",
	"IDS_LCKSCN_BODY_BOOK_COVER_M_LOCKSCREEN_NAME",
	"IDS_LCKSCN_BODY_MAGAZINE_M_LOCKSCREEN_NAME",
	"IDS_LCKSCN_BODY_DIARY",
	"IDS_LCKSCN_BODY_FUN_M_LOCKSCREEN_NAME",
	"IDS_LCKSCN_BODY_POLAROID_M_LOCKSCREEN_NAME",
	"IDS_SKM_BODY_WEATHER_M_IDEA_SKETCH",
	"IDS_ST_BODY_STYLE",
	"IDS_ST_BODY_NONE",
	"IDS_ST_MBODY_SWIPE",
	"IDS_ST_BODY_MOTION",
	"Face and voice",
	"IDS_ST_BODY_SIMPLE_PASSWORD",
	"IDS_ST_BODY_PASSWORD",
	"IDS_ST_POP_LOCK_SCREEN_WALLPAPER_CHANGED_ABB",
	"IDS_LCKSCN_MBODY_SET_WAKE_UP_COMMAND",
	"IDS_LCKSCN_BODY_EDIT_PERSONAL_MESSAGE",
	"IDS_COM_BODY_EDIT",
	"IDS_LCKSCN_BODY_YOU_CAN_INPUT_TEXT_ON_THE_LOCK_SCREEN_AND_EDIT_IT_BY_SELECTING_EDIT_PERSONAL_MESSAGE",
	"IDS_ST_HEADER_UNLOCK_EFFECT",
	"IDS_LCKSCN_OPT_CONFETTI_M_EFFECT",
	"IDS_LCKSCN_OPT_GEOMETRIC_M_EFFECT",
	"IDS_LCKSCN_OPT_STARDUST_M_EFFECT",
	"IDS_LCKSCN_OPT_GLOW_WORMS_M_EFFECT",
	"IDS_STORYALBUM_OPT_EDIT_TITLE",
	"IDS_LCKSCN_HEADER_SPECIAL_DAY_WALLPAPERS_ABB",
	"IDS_ST_HEADER_WEATHER_EFFECT_ABB",
	"IDS_LCKSCN_BODY_CUSTOMISE_THE_STYLE_OF_YOUR_LOCK_SCREEN",
	"IDS_ST_MBODY_FINGERPRINT",
	"IDS_ST_BODY_AUTO_LOCK",
	"IDS_ST_BODY_CAMERA_SHORTCUT",
	"IDS_ST_BODY_ADDITIONAL_INFORMATION_ABB",
	"IDS_ST_BODY_SHOW_INFORMATION_ABOUT_THE_WEATHER_AND_YOUR_STEP_COUNT_ON_THE_LOCK_SCREEN",
	"IDS_SKM_BODY_WEATHER_M_IDEA_SKETCH",
	"IDS_ST_BUTTON2_PEDOMETER",
	"IDS_LCKSCN_BODY_SHOW_INFORMATION_ABOUT_THE_WEATHER_HBASED_ON_THE_WEATHER_APPLICATION_MSG",
	"IDS_ST_BODY_DISPLAY_A_WEATHER_EFFECT_ON_THE_LOCK_SCREEN",
	"IDS_IM_BODY_HELP_TEXT",//90
	"IDS_ST_BODY_SHOW_HELP_TEXT_ON_LOCK_SCREEN",
	"IDS_LCKSCN_BODY_THEMED_WALLPAPER_WILL_BE_APPLIED_ON_THE_LOCK_SCREEN_FOR_RUSSIAN_PUBLIC_HOLIDAYS",
	"IDS_LCKSCN_MBODY_SWIPE_THEN_ENTER_PASSWORD",
	"IDS_LCKSCN_BODY_SET_THE_DEVICE_TO_SHOW_THE_KEYBOARD_AFTER_YOU_SWIPE_THE_LOCK_SCREEN",
};

Evas_Object *lockscreen_options_util_create_navigation(Evas_Object * parent)
{
	Evas_Object *navi_bar = NULL;

	if (parent == NULL) {
		LOCKOPTIONS_WARN("Parent is null.");
		return NULL;
	}

	navi_bar = elm_naviframe_add(parent);
	if (navi_bar == NULL) {
		LOCKOPTIONS_ERR("Cannot add naviagtionbar.");
		return NULL;
	}

	eext_object_event_callback_add(navi_bar, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, NULL);

	elm_object_part_content_set(parent, "elm.swallow.content", navi_bar);

	evas_object_show(navi_bar);

	return navi_bar;
}

Evas_Object *lockscreen_options_util_create_layout(Evas_Object * parent,
						   const char *file,
						   const char *group)
{
	Evas_Object *layout = NULL;

	if (parent == NULL) {
		LOCKOPTIONS_WARN("Parent is null.");
		return NULL;
	}

	layout = elm_layout_add(parent);
	if (layout == NULL) {
		LOCKOPTIONS_ERR("Cannot add layout.");
		return NULL;
	}

	if ((file != NULL) && (group != NULL)) {
		elm_layout_file_set(layout, file, group);
	}

	evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND,
					 EVAS_HINT_EXPAND);

	evas_object_show(layout);

	return layout;
}

char *lockscreen_optoins_get_string(int id)
{
	//LOCKOPTIONS_DBG("get string id : %d\n", id);

	char *str = NULL;

	if (id < IDS_LOCKSCREEN_OPTIONS_SYSTEM_STRING_MAX) {
		/* get system string */
		str = dgettext("sys_string", sys_str_table[id]);
	} else {
		/* get application string */
		str =
		    dgettext(PKGNAME,
			     app_str_table[id -
					   IDS_LOCKSCREEN_OPTIONS_SYSTEM_STRING_MAX]);
	}

	//LOCKOPTIONS_DBG("get string : %s\n", str);

	return str;
}

static char *_lockscreen_options_submenu_gl_label_get(void *data,
						   Evas_Object * obj,
						   const char *part)
{
	if (data == NULL || part == NULL)
		return NULL;

	lockscreen_menu_item_info *lockoption_data =
	    (lockscreen_menu_item_info *) data;

	if ((strcmp(part, "elm.text") == 0) ||
	    (strcmp(part, "elm.text.1") == 0)) {
		return
		    strdup(lockscreen_optoins_get_string
			   (lockoption_data->stringId));
	}
	else{
		return NULL;
	}

}

static Evas_Object *_lockscreen_options_submenu_gl_icon_get(void *data,
							 Evas_Object * obj,
							 const char *part)
{
	if (data == NULL || obj == NULL)
		return NULL;

	lockscreen_menu_item_info *lockoption_data =
	    (lockscreen_menu_item_info *) data;

	Evas_Object *check;
	int value = 0;

	check = elm_check_add(obj);
	elm_object_style_set(check, "on&off");
	evas_object_show(check);

	elm_check_state_set(check, value);

	evas_object_pass_events_set(check, 1);
	evas_object_propagate_events_set(check, 0);

	lockoption_data->check = check;

	return check;
}

void _lockscreen_options_submenu_gl_item(Elm_Gen_Item_Class * item)
{
	item->item_style = LOCKSCREEN_GENLIST_STYLE_1TEXT1ICON;
	item->func.text_get = _lockscreen_options_submenu_gl_label_get;
	item->func.content_get = _lockscreen_options_submenu_gl_icon_get;
	item->func.state_get = NULL;
	item->func.del = NULL;
}

void remove_unused_string(char *text, char *unused_string)
{
	if(text != NULL && unused_string != NULL && unused_string[0] != '\0') {
		LOCKOPTIONS_DBG("remove unused string %s in string %s", unused_string, text);
		char *p = NULL;
		int len = 0;
		len = strlen(unused_string);

		p = strstr(text, unused_string);
		while(p != NULL) {
			while(*(p + len) != '\0') {
				*p = *(p + len);
				*(p + len) = '\0';
				p++;
			}
			*p = '\0';
			p = strstr(text, unused_string);
		}
		LOCKOPTIONS_DBG("the string after remove is %s", text);
	}
}

void replace_unused_string_with_char(char *text, char *unused_string, char c)
{
	if(text != NULL && unused_string != NULL && unused_string[0] != '\0') {
		LOCKOPTIONS_DBG("remove unused string %s in string %s", unused_string, text);
		char *p = NULL;
		int len = 0;
		len = strlen(unused_string);

		p = strstr(text, unused_string);
		while(p != NULL) {
			*p = c;
			p++;
			while(*(p + len - 1) != '\0') {
				*p = *(p + len - 1);
				*(p + len - 1) = '\0';
				p++;
			}
			*p = '\0';
			p = strstr(text, unused_string);
		}
		LOCKOPTIONS_DBG("the string after remove is %s", text);
	}
}

int get_vconf_screenreader(void)
{
	int is_screenreader = -1;
	if(vconf_get_bool(VCONFKEY_SETAPPL_ACCESSIBILITY_TTS, &is_screenreader) < 0) {
		return EINA_FALSE;
	}
	return is_screenreader;
}

void *lockscreen_options_tts_reader_object_get(void *obj, lockscreen_object_type_e type, const char *part, void *parent)
{
	Evas_Object *to = NULL;
	Evas_Object *ao = NULL;

	if(obj == NULL) {
		LOCKOPTIONS_ERR("invalid parameter");
		return NULL;
	}

	if(type == LOCKSCREEN_READER_OBJ_TYPE_EDJ_OBJECT && !part) {
		LOCKOPTIONS_ERR("invalid parameter");
		return NULL;
	}

	switch (type) {
		case LOCKSCREEN_READER_OBJ_TYPE_ELM_OBJECT:
			if(part != NULL) {
				to = (Evas_Object *)elm_object_part_content_get(obj, part);
				ao = (Evas_Object *)to;
			} else {
				ao = (Evas_Object *)obj;
			}
			break;

		case LOCKSCREEN_READER_OBJ_TYPE_EDJ_OBJECT:
			to = (Evas_Object *)edje_object_part_object_get(_EDJ(obj), part);
			break;

		default:	// evas, icon
			to = (Evas_Object *)obj;
	}

	if (!ao && to && parent) {		// edj, evas, icon, elm_object_item
		ao = elm_access_object_get(to);
		if (ao == NULL) {
			ao = elm_access_object_register(to, parent);
		}
	}

	return ao;
}
void *lockscreen_options_tts_get_focus_object(void *parent)
{
	if(parent == NULL)
		return NULL;
	Evas_Object *focus = elm_button_add(parent);
	if(focus == NULL) {
		LOCKOPTIONS_DBG("elm_button_add() failed");
		return NULL;
	}
	elm_object_style_set(focus, "focus");
	return focus;
}

void lockscreen_options_set_tts_info(Evas_Object* obj, const char* label,
			  const char* traits, const char *state,
			  const char* guide)
{
	if (label)
		elm_access_info_set(obj, ELM_ACCESS_INFO, label);

	if (traits)
		elm_access_info_set(obj, ELM_ACCESS_TYPE, traits);

	if (state)
		elm_access_info_set(obj, ELM_ACCESS_STATE, state);
	if (guide)
	{
		elm_access_info_set(obj, ELM_ACCESS_CONTEXT_INFO, NULL);
		elm_access_info_set(obj, ELM_ACCESS_CONTEXT_INFO, guide);
	}
}

static void _launch_result_password_ug_cb(app_control_h ug, app_control_h reply, app_control_result_e result, void *data)
{
	LOCKOPTIONS_TRACE_BEGIN;
	if(data == NULL)
		return;
	lockscreen_options_ug_data *ad = (lockscreen_options_ug_data *) data;	/* ad is point to priv */

	if(!result)
	{
		LOCKOPTIONS_DBG(" !result ");
		char *string_result = NULL;
		char *current = NULL;
		app_control_get_extra_data(reply, "result", &string_result);
		if(string_result == NULL)
			return;
		LOCKOPTIONS_DBG("_launch_result_password_ug_cb string_result is %s.",string_result);
		int ret_int = 0;
		if(strcmp(string_result, "Cancel") == 0)
		{
			ret_int = 0;
		}
		else if(strcmp(string_result, "SETTING_PW_TYPE_ENTER_LOCK_TYPE") == 0 || strcmp(string_result, "SETTING_PW_TYPE_VERIFY_FP_ALT_PASSWORD") == 0)
		{
			ret_int = 1;
		}
		free(string_result);

		if(ret_int == 1)
		{
			LOCKOPTIONS_DBG(" SETTING_PW_TYPE_ENTER_LOCK_TYPE ||SETTING_PW_TYPE_VERIFY_FP_ALT_PASSWORD  _launch_result_password_ug_cb");
			app_control_h svc;
			if (app_control_create(&svc)) {
				return;
			}
			app_control_get_extra_data(reply, "current", &current);
			if(current){
				app_control_add_extra_data(svc, "current", current);
				free(current);
			}
			launch_ug("setting-locktype-efl", svc, ad);
			app_control_destroy(svc);
		}
	}
}

void launch_ug(char* ug_name, app_control_h svc_mt_ug, void *data)
{
	LOCKOPTIONS_DBG("_launch_ug. %s \n", ug_name);
	if(data == NULL){
		LOCKOPTIONS_ERR("The data (ug_data)is NULL.");
		return;
	}
	if(ug_name == NULL) {
		return;
	}

	app_control_set_app_id(svc_mt_ug, ug_name);
	app_control_set_launch_mode(svc_mt_ug, APP_CONTROL_LAUNCH_MODE_GROUP);

	if(strcmp(ug_name, "setting-password-efl") == 0) {
		app_control_send_launch_request(svc_mt_ug, _launch_result_password_ug_cb, data);
	} else {
		app_control_send_launch_request(svc_mt_ug, NULL, data);
	}

	LOCKOPTIONS_DBG("Launch ug end. %s \n", ug_name);
}

Evas_Object *_create_check_on_naviframe(Evas_Object *parent,char * vconf,Evas_Smart_Cb func, void *data)
{
	lockscreen_menu_item_info *lockoption_data =
	    (lockscreen_menu_item_info *) data;

	Evas_Object *check;
	int value = 0;

	check = elm_check_add(parent);
	elm_object_style_set(check, "on&off");
	evas_object_show(check);

	vconf_get_bool(vconf, &value);
	elm_check_state_set(check, value);

	evas_object_pass_events_set(check, 1);
	evas_object_propagate_events_set(check, 0);

	evas_object_smart_callback_add(check, "changed",
				       func,
				       lockoption_data);

	return check;
}

