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


#include <vconf.h>
#include <vconf-keys.h>
#include <app_control.h>
#include <app.h>

#include "lockscreen-options.h"
#include "lockscreen-options-debug.h"
#include "lockscreen-options-main.h"
#include "lockscreen-options-util.h"
#include "app-shortcut.h"

#define LOCKSCREEN_PACKAGE_NAME				"org.tizen.lockscreen"

#define LOCKSCREEN_OPTOINS_GENLIST_ITEM_NONE 1
#define LOCKSCREEN_OPTOINS_GENLIST_ITEM_SWIPE 2
#define LOCKSCREEN_OPTOINS_GENLIST_ITEM_PW 2


#define CITY_BUF_SIZE 		128
#define GMT_BUF_SIZE        10

#define IS_LOCKTYPE(a,b) \
{	\
	if(a == SETTING_SCREEN_LOCK_TYPE_NONE)	\
		b = 0;	\
	if(	a == SETTING_SCREEN_LOCK_TYPE_FINGERPRINT ||	\
		a == SETTING_SCREEN_LOCK_TYPE_SIMPLE_PASSWORD ||	\
		a == SETTING_SCREEN_LOCK_TYPE_PASSWORD)	\
		b = 1;	\
	if(a == SETTING_SCREEN_LOCK_TYPE_OTHER)	\
		b = 2;	\
	if(a == SETTING_SCREEN_LOCK_TYPE_MOTION) \
		b = 3 ; \
	if(a == SETTING_SCREEN_LOCK_TYPE_SWIPE)	\
		b = 4;	\
}

extern const char *chosen_app;

static void _lockscreen_options_register_vconf_change(lockscreen_options_ug_data * ug_data);
void _lockscreen_options_unregister_vconf_change();

static void _lockscreen_options_app_shortcut_cb(void *data, Evas_Object * obj, void *event_info);
static void _lockscreen_options_locktype_cb(void *data, Evas_Object * obj, void *event_info);

static lockscreen_menu_item_info lockscreen_options_menu_item_none[] = {
	{ENUM_LOCKSCREEN_GENLIST_STYLE_2TEXT, IDS_LOCKSCREEN_OPTIONS_LOCK_SCREEN_TYPE, NULL, _lockscreen_options_locktype_cb, NULL},
};

static lockscreen_menu_item_info lockscreen_options_menu_item_pw[] = {
	{ENUM_LOCKSCREEN_GENLIST_STYLE_2TEXT, IDS_LOCKSCREEN_OPTIONS_LOCK_SCREEN_TYPE, NULL, _lockscreen_options_locktype_cb, NULL}, //type
	{ENUM_LOCKSCREEN_GENLIST_STYLE_2TEXT, IDS_LOCKSCREEN_OPTIONS_APP_SHORTCUT, NULL, _lockscreen_options_app_shortcut_cb, NULL},
};

static lockscreen_menu_item_info lockscreen_options_menu_item_swipe[] = {
	{ENUM_LOCKSCREEN_GENLIST_STYLE_2TEXT, IDS_LOCKSCREEN_OPTIONS_LOCK_SCREEN_TYPE, NULL, _lockscreen_options_locktype_cb, NULL,NULL}, //type
	{ENUM_LOCKSCREEN_GENLIST_STYLE_2TEXT, IDS_LOCKSCREEN_OPTIONS_APP_SHORTCUT, NULL, _lockscreen_options_app_shortcut_cb, NULL},
};

static Elm_Gen_Item_Class itc_menu_1text;
static Elm_Gen_Item_Class itc_menu_1text1icon;
static Elm_Gen_Item_Class itc_title_2text;
static Elm_Gen_Item_Class itc_menu_2text1icon;
static Elm_Gen_Item_Class itc_multiline_text;


static Elm_Object_Item *genlist_selected_item = NULL;
static Evas_Object *g_genlist = NULL;

static Elm_Object_Item *item_unlock = NULL;
static Elm_Object_Item *item_type = NULL;
static Elm_Object_Item *item_security = NULL;

static Evas_Object *radio_group = NULL;

Evas_Object *item_radio = NULL;

static void _lockscreen_options_app_shortcut_cb(void *data, Evas_Object * obj, void *event_info)
{
    LOCKOPTIONS_TRACE_BEGIN;
    create_app_shortcut_view(data);
}

static void _lockscreen_options_locktype_cb(void *data, Evas_Object * obj, void *event_info)
{
	int lock_type = 0;
	app_control_h svc_mt_ug = NULL;
	char* ug_name = NULL;
	vconf_get_int(VCONFKEY_SETAPPL_SCREEN_LOCK_TYPE_INT, &lock_type);
	if(!app_control_create(&svc_mt_ug)) {
		if(lock_type == SETTING_SCREEN_LOCK_TYPE_PASSWORD
			|| lock_type == SETTING_SCREEN_LOCK_TYPE_SIMPLE_PASSWORD)
		{
			LOCKOPTIONS_DBG("SETTING_SCREEN_LOCK_TYPE_PASSWORD || SETTING_SCREEN_LOCK_TYPE_SIMPLE_PASSWORD");
			app_control_add_extra_data(svc_mt_ug, "viewtype", "SETTING_PW_TYPE_ENTER_LOCK_TYPE");
			ug_name = "setting-password-efl";
			launch_ug(ug_name, svc_mt_ug, data);
			app_control_destroy(svc_mt_ug);
		} else {
			LOCKOPTIONS_DBG("setting-locktype-efl");
			ug_name = "setting-locktype-efl";
			launch_ug(ug_name, svc_mt_ug, data);
			app_control_destroy(svc_mt_ug);
		}
	}
}

void lockscreen_options_locktype_create_view(lockscreen_options_ug_data * ug_data)
{
	LOCKOPTIONS_TRACE_BEGIN;

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
		} else {
			ug_name = "setting-locktype-efl";
		}
	}

	launch_ug(ug_name, svc_mt_ug, ug_data);
	app_control_destroy(svc_mt_ug);

}

int _lockscreen_options_txt_list_create(char *filename)
{
	LOCKOPTIONS_TRACE_BEGIN;

	LOCKOPTIONS_DBG("filename=%s", filename);
	if(filename == NULL)
		return -1;

	if(access(filename, 0) == -1)
	{
		LOCKOPTIONS_DBG("%s is not exist.", filename);
		FILE *file = fopen(filename, "w");
		if(!file)
		{
			LOCKOPTIONS_DBG("fopen file failed");
			return -1;
		}
		fclose(file);
	}
	LOCKOPTIONS_TRACE_END;
	return 0;
}

static char *_lockscreen_options_main_gl_label_get(void *data, Evas_Object * obj, const char *part)
{
	if (data == NULL || part == NULL)
		return NULL;

	lockscreen_menu_item_info *lockoption_data = (lockscreen_menu_item_info *) data;

	const Elm_Genlist_Item_Class *itc = elm_genlist_item_item_class_get(lockoption_data->item);
	if(NULL == itc)
	{
		LOCKOPTIONS_DBG("itc is null");
		return NULL;
	}

		if (!strcmp(part, "elm.text") || !strcmp(part, "elm.text.main.left") || !strcmp(part, "elm.text.main.left.top"))
		{	/* title */
			return strdup(lockscreen_options_get_string(lockoption_data->stringId));
		}
		else if (!strcmp(part, "elm.text.sub") || !strcmp(part, "elm.text.sub.left.bottom"))
		{	/* bottom or right */
			if(lockoption_data->stringId == IDS_LOCKSCREEN_OPTIONS_LOCK_SCREEN_TYPE)
			{
				int lock_type = 0;
				vconf_get_int(VCONFKEY_SETAPPL_SCREEN_LOCK_TYPE_INT, &lock_type);
				char* type_str = NULL;
				switch(lock_type)
				{
					case SETTING_SCREEN_LOCK_TYPE_NONE:
						type_str = lockscreen_options_get_string(IDS_LOCKSCREEN_OPTIONS_STYLE_NONE);
						break;
					case SETTING_SCREEN_LOCK_TYPE_SWIPE:
						type_str = lockscreen_options_get_string(IDS_LOCKSCREEN_OPTIONS_STYLE_SWIPE);
						break;
					case SETTING_SCREEN_LOCK_TYPE_MOTION:
						type_str = lockscreen_options_get_string(IDS_LOCKSCREEN_OPTIONS_STYLE_MOTION);
						break;
					case SETTING_SCREEN_LOCK_TYPE_FACE_AND_VOICE:
						type_str = lockscreen_options_get_string(IDS_LOCKSCREEN_OPTIONS_STYLE_VOICE);
						break;
					case SETTING_SCREEN_LOCK_TYPE_SIMPLE_PASSWORD:
						//type_str = lockscreen_optoins_get_string(IDS_LOCKSCREEN_OPTIONS_STYLE_SIMPLE_PASSWORD);
						type_str = "PIN";
						break;
					case SETTING_SCREEN_LOCK_TYPE_PASSWORD:
						type_str = lockscreen_options_get_string(IDS_LOCKSCREEN_OPTIONS_STYLE_PASSWORD);
						break;
					case SETTING_SCREEN_LOCK_TYPE_FINGERPRINT:
						type_str = lockscreen_options_get_string(IDS_LOCKSCREEN_OPTIONS_STYLE_FINGERPRINT);
						break;
					case SETTING_SCREEN_LOCK_TYPE_AUTO_LOCK:
						type_str = lockscreen_options_get_string(IDS_LOCKSCREEN_OPTIONS_STYLE_AUTOLOCK);
						break;
					case SETTING_SCREEN_LOCK_TYPE_OTHER:
						return vconf_get_str(VCONFKEY_SETAPPL_3RD_LOCK_PKG_NAME_STR);
					default:
						return NULL;
				}
				return strdup(type_str);
			}
			if(lockoption_data->stringId == IDS_LOCKSCREEN_OPTIONS_APP_SHORTCUT)
			{
			    return chosen_app ? strdup(chosen_app) : NULL;
			}

		} else if (!strcmp(part, "elm.text.multiline")) {	/* title */
				LOCKOPTIONS_DBG("elm.text.multiline");
				return strdup(lockscreen_options_get_string(lockoption_data->stringId));
		} else {
			return NULL;
		}

	return NULL;

}

static void _lockscreen_options_set_menu_status(int stringId, int value)
{
	LOCKOPTIONS_TRACE_BEGIN;
	int ret = 0;
	switch (stringId) {
	case IDS_LOCKSCREEN_OPTIONS_APP_SHORTCUT:
		LOCKOPTIONS_DBG("IDS_LOCKSCREEN_OPTIONS_CAMERA_SHORTCUT : value = %d", value);
		ret = vconf_set_bool(VCONFKEY_LOCKSCREEN_CAMERA_QUICK_ACCESS, value);
		break;
	default:
		LOCKOPTIONS_DBG("NO VALID STRINGID %d", stringId);
		break;

	}

	if (ret != 0) {
		LOCKOPTIONS_ERR("Failed to set vconfkey %d!", stringId);
	}
}

static bool _lockscreen_options_get_menu_status(int stringId)
{
	LOCKOPTIONS_TRACE_BEGIN;
	int status = 0;
	int ret = 0;

	switch (stringId) {
	case IDS_LOCKSCREEN_OPTIONS_APP_SHORTCUT:
		ret = vconf_get_bool(VCONFKEY_LOCKSCREEN_CAMERA_QUICK_ACCESS, &status);
		LOCKOPTIONS_DBG("VCONFKEY_LOCKSCREEN_CAMERA_QUICK_ACCESS = %d", status);
		break;
	default:
		LOCKOPTIONS_DBG("NO VALID INDEX %d", stringId);
		break;
	}

	if (ret == -1) {
		LOCKOPTIONS_ERR("Failed to get vconfkey %d!", stringId);
		return 0;
	}
	return status;
}

static void _lockscreen_options_check_changed_cb(void *data, Evas_Object * obj,
						 void *event_info)
{
	if (data == NULL || obj == NULL)
		return;

	lockscreen_menu_item_info *lockoption_data = (lockscreen_menu_item_info *) data;

	Eina_Bool ret;
	int value = 0;

	ret = elm_check_state_get(obj);

	LOCKOPTIONS_DBG("_lockscreen_options_check_changed_cb : %s",
			ret == EINA_TRUE ? "ON" : "OFF");

	if (ret == EINA_TRUE) {
		/* set on */
		value = 1;
	} else {
		/* set off */
		value = 0;
	}

	_lockscreen_options_set_menu_status(lockoption_data->stringId, value);
}

static Evas_Object *_lockscreen_options_main_gl_icon_get(void *data,
							 Evas_Object * obj,
							 const char *part)
{
	if (data == NULL || obj == NULL)
		return NULL;
	lockscreen_menu_item_info *lockoption_data = (lockscreen_menu_item_info *) data;

	if (!strcmp(part, "elm.swallow.end") || !strcmp(part, "elm.icon")) {
		int value = 0;
		Evas_Object *check = elm_check_add(obj);
		elm_object_style_set(check, "on&off");
		evas_object_show(check);

		value = _lockscreen_options_get_menu_status(lockoption_data->stringId);
		elm_check_state_set(check, value);

		evas_object_pass_events_set(check, 1);
		evas_object_propagate_events_set(check, 0);

		evas_object_smart_callback_add(check, "changed", _lockscreen_options_check_changed_cb, lockoption_data);

		lockoption_data->check = check;
		return check;
	}

	return NULL;
}

static char *_lockscreen_options_main_gl_text_get(void *data,
							Evas_Object * obj,
							const char *part)
{
	LOCKOPTIONS_TRACE_BEGIN;
	if (data == NULL || obj == NULL)
		return NULL;

	lockscreen_menu_item_info *lockoption_data =
		(lockscreen_menu_item_info *) data;
	if (!strcmp(part, "elm.text.multiline")) {
		return strdup(lockscreen_options_get_string
				(lockoption_data->stringId));
	}
	else
	{
		LOCKOPTIONS_DBG("it is not multiline");
	}
	LOCKOPTIONS_TRACE_END;
	return NULL;
}


static void _lockscreen_options_main_gl_del(void *data, Evas_Object * obj)
{

}

static void _lockscreen_options_main_gl_sel(void *data, Evas_Object * obj, void *event_info)
{
	LOCKOPTIONS_DBG(" _lockscreen_options_main_gl_sel");
	if (data == NULL)
		return;

	lockscreen_menu_item_info *lockoption_data = NULL;
	elm_genlist_item_selected_set((Elm_Object_Item *) event_info, EINA_FALSE);

	Elm_Object_Item *item = (Elm_Object_Item *) event_info;
	genlist_selected_item = item;
	lockoption_data = (lockscreen_menu_item_info *) elm_object_item_data_get(item);
	if (lockoption_data == NULL)
	{
		return;
	}

	if (lockoption_data->stringId == IDS_LOCKSCREEN_OPTIONS_APP_SHORTCUT
		|| lockoption_data->stringId == IDS_LOCKSCREEN_OPTIONS_SWIPE_PW)
	{
		elm_genlist_item_update(item);
	}

	if (lockoption_data->func != NULL) {
		lockoption_data->func(data, obj, event_info);
	}
}

static void _lockscreen_options_create_gl_item(Elm_Gen_Item_Class * item,
					       int glStyle)
{
	LOCKOPTIONS_DBG("_lockscreen_options_create_gl_item, glStyle = %d", glStyle);
	if (glStyle == ENUM_LOCKSCREEN_GENLIST_STYLE_1TEXT1ICON) {
		item->item_style = "type1";
		item->func.text_get = _lockscreen_options_main_gl_label_get;
		item->func.content_get = _lockscreen_options_main_gl_icon_get;
		item->func.state_get = NULL;
		item->func.del = _lockscreen_options_main_gl_del;
	} else if (glStyle == ENUM_LOCKSCREEN_GENLIST_STYLE_1TEXT) {
		item->item_style = "1line";
		item->func.text_get = _lockscreen_options_main_gl_label_get;
		item->func.content_get = NULL;
		item->func.state_get = NULL;
		item->func.del = _lockscreen_options_main_gl_del;
	} else if (glStyle == ENUM_LOCKSCREEN_GENLIST_STYLE_1TEXT1ICON_DIVIDER) {
		item->item_style = "1line";
		item->func.text_get = _lockscreen_options_main_gl_label_get;
		item->func.content_get = _lockscreen_options_main_gl_icon_get;
		item->func.state_get = NULL;
		item->func.del = NULL;
	} else if (glStyle == ENUM_LOCKSCREEN_GENLIST_STYLE_2TEXT1ICON_DIVIDER) {
		item->item_style = LOCKSCREEN_GENLIST_STYLE_2TEXT1ICON_DIVIDER;
		item->func.text_get = _lockscreen_options_main_gl_label_get;
		item->func.content_get = _lockscreen_options_main_gl_icon_get;
		item->func.state_get = NULL;
		item->func.del = NULL;
	} else if (glStyle == ENUM_LOCKSCREEN_GENLIST_STYLE_HELP) {
		item->item_style = "1line";
		item->func.text_get = _lockscreen_options_main_gl_text_get;
		item->func.content_get = NULL;
		item->func.state_get = NULL;
		item->func.del = _lockscreen_options_main_gl_del;
	} else if (glStyle == ENUM_LOCKSCREEN_GENLIST_STYLE_TITLE) {
		item->item_style = LOCKSCREEN_GENLIST_STYLE_TITLE;
		item->func.text_get = _lockscreen_options_main_gl_label_get;
		item->func.content_get = NULL;
		item->func.state_get = NULL;
		item->func.del = _lockscreen_options_main_gl_del;
	} else if(glStyle == ENUM_LOCKSCREEN_GENLIST_STYLE_2TEXT) {
		LOCKOPTIONS_DBG("_lockscreen_options_create_gl_item  2line.top");
		item->item_style = "type1";
		item->func.text_get = _lockscreen_options_main_gl_label_get;
		item->func.content_get = NULL;
		item->func.state_get = NULL;
		item->func.del = _lockscreen_options_main_gl_del;
	} else if(glStyle == ENUM_LOCKSCREEN_GENLIST_STYLE_2TEXT_EXPANDABLE) {
		item->item_style = LOCKSCREEN_GENLIST_STYLE_2TEXT_EXPANDABLE;
		item->func.text_get = _lockscreen_options_main_gl_label_get;
		item->func.content_get = NULL;
		item->func.state_get = NULL;
		item->func.del = _lockscreen_options_main_gl_del;
	}else if(glStyle == ENUM_LOCKSCREEN_GENLIST_STYLE_2TEXT1ICON)
	{
		item->item_style = "multiline_sub.main";
		item->func.text_get = _lockscreen_options_main_gl_label_get;
		//item->func.content_get = _lockscreen_options_main_gl_icon_get;
		item->func.state_get = NULL;
		item->func.del = _lockscreen_options_main_gl_del;
	}else if(glStyle == ENUM_LOCKSCREEN_GENLIST_STYLE_MULTILINE_SUB)
	{
		item->item_style = "multiline_sub";
		item->func.text_get = _lockscreen_options_main_gl_label_get;
		item->func.content_get = NULL;
		item->func.state_get = NULL;
		item->func.del = _lockscreen_options_main_gl_del;
	}
	else {
		LOCKOPTIONS_DBG("_lockscreen_options_create_gl_item FAIL\n");
	}
}

static Eina_Bool _pop_cb(void *data, Elm_Object_Item *it)
{
	LOCKOPTIONS_DBG("%s, %d", __func__, __LINE__);

	/*do something before destroy*/
	_lockscreen_options_unregister_vconf_change();

	lockscreen_options_ug_data *ug_data =
		(lockscreen_options_ug_data *) data;

	if (ug_data == NULL)
		return EINA_FALSE;

	ui_app_exit();

	return EINA_FALSE;
}

static void _gl_exp(void *data, Evas_Object *obj, void *event_info)
{
	LOCKOPTIONS_TRACE_BEGIN;
	radio_group = elm_radio_add(g_genlist);
	elm_radio_state_value_set(radio_group, 0);
	elm_radio_value_set(radio_group, 0);

	Elm_Object_Item *it = (Elm_Object_Item*)event_info;
	elm_object_item_widget_get(it);

	LOCKOPTIONS_TRACE_END;
}

static void _gl_con(void *data, Evas_Object *obj, void *event_info)
{
	LOCKOPTIONS_TRACE_BEGIN;
	Elm_Object_Item *item = (Elm_Object_Item*)event_info;
	elm_genlist_item_subitems_clear(item);

	elm_object_item_signal_emit(item, "elm,state,normal", "");
	item = NULL;
	LOCKOPTIONS_TRACE_END;
}

static void lockscreen_options_main_create_genlist(Evas_Object *genlist, lockscreen_options_ug_data * ug_data)
{
    LOCKOPTIONS_TRACE_BEGIN;
    int genlist_show_mode = 0;
	int genlist_num = 0;
	int i = 0;
	int lock_type = 0;
	Elm_Object_Item *item = NULL;
	if(genlist == NULL)
		return;
	evas_object_smart_callback_add(genlist, "expanded", _gl_exp, genlist);
	evas_object_smart_callback_add(genlist, "contracted", _gl_con, genlist);

	vconf_get_int(VCONFKEY_SETAPPL_SCREEN_LOCK_TYPE_INT, &lock_type);
	IS_LOCKTYPE(lock_type, genlist_show_mode);
	LOCKOPTIONS_DBG("genlist_show_mode = %d", genlist_show_mode);
	switch(genlist_show_mode) {
		case 0:
			genlist_num = LOCKSCREEN_OPTOINS_GENLIST_ITEM_NONE;
			break;
		case 1:
			genlist_num = LOCKSCREEN_OPTOINS_GENLIST_ITEM_PW;
			break;
		case 2:
			genlist_num = LOCKSCREEN_OPTOINS_GENLIST_ITEM_NONE;
			break;
		case 4:
			genlist_num = LOCKSCREEN_OPTOINS_GENLIST_ITEM_SWIPE;
			break;
	}

	for (i = 0; i < genlist_num; i++) {
		lockscreen_menu_item_info *menu_item = NULL;
		Elm_Gen_Item_Class *itc = NULL;
		if(genlist_show_mode == 0) {
			lockscreen_options_menu_item_none[i].data = ug_data;
			menu_item = &lockscreen_options_menu_item_none[i];
		}else if(genlist_show_mode == 1) {
			lockscreen_options_menu_item_pw[i].data = ug_data;
			menu_item = &lockscreen_options_menu_item_pw[i];
		}else if(genlist_show_mode == 2 ) {
			lockscreen_options_menu_item_none[i].data = ug_data;
			menu_item = &lockscreen_options_menu_item_none[i];
		} else if(genlist_show_mode == 4) {
			lockscreen_options_menu_item_swipe[i].data = ug_data;
			menu_item = &lockscreen_options_menu_item_swipe[i];
		}

		if(menu_item != NULL) {
			switch(menu_item->glStyle) {
				case ENUM_LOCKSCREEN_GENLIST_STYLE_1TEXT1ICON: {
					LOCKOPTIONS_DBG("ENUM_LOCKSCREEN_GENLIST_STYLE_1TEXT1ICON 000");
					itc = &(itc_menu_1text1icon);
					item = elm_genlist_item_append(genlist,
							itc,
							menu_item, NULL,
							ELM_GENLIST_ITEM_NONE,
							_lockscreen_options_main_gl_sel,
							ug_data);
					if(IDS_LOCKSCREEN_OPTIONS_SWIPE_PW == menu_item->stringId) {
						item_security = item;
					}
					menu_item->item = item;
					break;
				}
				case ENUM_LOCKSCREEN_GENLIST_STYLE_1TEXT: {
					LOCKOPTIONS_DBG("ENUM_LOCKSCREEN_GENLIST_STYLE_1TEXT");
					itc = &(itc_menu_1text);
					item = elm_genlist_item_append(genlist,
							itc,
							menu_item, NULL,
							ELM_GENLIST_ITEM_NONE,
							_lockscreen_options_main_gl_sel,
							ug_data);

					menu_item->item = item;
					break;
				}
				case ENUM_LOCKSCREEN_GENLIST_STYLE_2TEXT: {
					LOCKOPTIONS_DBG("ENUM_LOCKSCREEN_GENLIST_STYLE_2TEXT");
					itc = &(itc_title_2text);
					item = elm_genlist_item_append(genlist,
							                       itc,
							                       menu_item,
							                       NULL,
							                       ELM_GENLIST_ITEM_NONE,
							                       _lockscreen_options_main_gl_sel,
							                       ug_data);
					if(item == NULL)
					{
						LOCKOPTIONS_WARN("elm_genlist_item_append() failed");
					}
					if(IDS_LOCKSCREEN_OPTIONS_LOCK_SCREEN_TYPE == menu_item->stringId)
					{
						item_type = item;
					}
					if(IDS_LOCKSCREEN_OPTIONS_APP_SHORTCUT == menu_item->stringId)
					{
					    item_type = item;
					}
					menu_item->item = item;
					break;
				}
				case ENUM_LOCKSCREEN_GENLIST_STYLE_MULTILINE_SUB: {
					LOCKOPTIONS_DBG("ENUM_LOCKSCREEN_GENLIST_STYLE_MULTILINE_SUB");
					itc = &(itc_multiline_text);
					item = elm_genlist_item_append(genlist,
							itc,
							menu_item, NULL,
							ELM_GENLIST_ITEM_NONE,
							NULL, ug_data);
					elm_genlist_item_select_mode_set(item, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);
					menu_item->item = item;
					break;
				}
				default: {
					LOCKOPTIONS_WARN("lockscreen option has no such type.");
					return;
				}
			}
		}
	}
	LOCKOPTIONS_DBG("lockscreen_options_main_create_genlist end");
}

static void _back_btn_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
	elm_naviframe_item_pop(data);
}

void lockscreen_options_main_create_view(lockscreen_options_ug_data * ug_data)
{
	LOCKOPTIONS_DBG("lockscreen_options_main_create_view begin\n");

	Evas_Object *navi_frame = ug_data->navi_frame;
	Evas_Object *back_button = NULL;
	Evas_Object *genlist = NULL;
	int lock_type = 0;

	if (navi_frame == NULL) {
		LOCKOPTIONS_WARN("navi_bar is null.");
		return;
	}

	vconf_get_int(VCONFKEY_SETAPPL_SCREEN_LOCK_TYPE_INT, &lock_type);
	ug_data->locktype = lock_type;

	_lockscreen_options_create_gl_item(&(itc_title_2text), ENUM_LOCKSCREEN_GENLIST_STYLE_2TEXT);
	_lockscreen_options_create_gl_item(&(itc_menu_1text), ENUM_LOCKSCREEN_GENLIST_STYLE_1TEXT);
	_lockscreen_options_create_gl_item(&(itc_menu_1text1icon), ENUM_LOCKSCREEN_GENLIST_STYLE_1TEXT1ICON);
	_lockscreen_options_create_gl_item(&(itc_menu_2text1icon), ENUM_LOCKSCREEN_GENLIST_STYLE_2TEXT1ICON);
	_lockscreen_options_create_gl_item(&(itc_multiline_text), ENUM_LOCKSCREEN_GENLIST_STYLE_MULTILINE_SUB);

	/* Create genlist */
	genlist = elm_genlist_add(navi_frame);
	g_genlist = genlist;
	elm_genlist_mode_set(genlist, ELM_LIST_COMPRESS);
	elm_object_style_set(genlist, "dialogue");

	lockscreen_options_main_create_genlist(genlist, ug_data);

	/* Set navigation objects and push */
	back_button = elm_button_add(navi_frame);
	elm_object_style_set(back_button, "naviframe/back_btn/default");
	evas_object_smart_callback_add(back_button, "clicked", _back_btn_clicked_cb, navi_frame);

	Elm_Object_Item *navi_item = elm_naviframe_item_push(navi_frame, "IDS_LCKSCN_HEADER_LOCK_SCREEN", back_button, NULL, genlist, NULL);	/* the same tile */
	elm_object_item_domain_text_translatable_set(navi_item, PKGNAME, EINA_TRUE);
	elm_naviframe_item_pop_cb_set(navi_item, _pop_cb, ug_data);
	ug_data->main_navi_item = navi_item;

	_lockscreen_options_register_vconf_change(ug_data);

	LOCKOPTIONS_DBG("lockscreen_options_main_create_view end");
}

void lockscreen_options_main_update_view()
{
	LOCKOPTIONS_DBG("%s, %d", __func__, __LINE__);
	if(g_genlist != NULL) {
		LOCKOPTIONS_DBG("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		Eina_Bool pass = evas_object_pass_events_get(g_genlist);
		if(pass == EINA_TRUE)
			LOCKOPTIONS_DBG("!!!!!!!!!!!!!EINA_TRUE");
		else
			LOCKOPTIONS_DBG("=============EINA_FALSE");
		evas_object_pass_events_set(g_genlist, EINA_FALSE);
		Elm_Object_Item *item = NULL;
		Elm_Object_Item *item_1 = elm_genlist_first_item_get(g_genlist);
		elm_genlist_item_update(item_1);
		while((item = elm_genlist_item_next_get(item_1)) != NULL) {
			elm_genlist_item_update(item);
			item_1 = item;
		}
	}
}

void lockscreen_options_main_create_view_update(keynode_t *node, void *data)
{
	LOCKOPTIONS_TRACE_BEGIN ;
	if(NULL == data){
		LOCKOPTIONS_DBG("---------NULL == ug_data");
		return ;
	}
	lockscreen_options_ug_data * ug_data = data;
	if(g_genlist != NULL) {
		elm_genlist_clear(g_genlist);
		evas_object_del(g_genlist);
		g_genlist = NULL;
	}
		lockscreen_options_main_create_view(ug_data);
}

static void _lockscreen_options_locktype_change_cb(keynode_t *node, void *data)
{
	lockscreen_options_ug_data * ug_data = data;
	int old_locktype = ug_data->locktype;
	int new_locktype = 0;
	int ret = 0;
	vconf_get_int(VCONFKEY_SETAPPL_SCREEN_LOCK_TYPE_INT, &new_locktype);
	LOCKOPTIONS_DBG("_lockscreen_options_locktype_change_cb,old type is %d, new type is %d",old_locktype, new_locktype);

	if(g_genlist != NULL) {
		item_unlock = NULL;
		elm_genlist_clear(g_genlist);
		evas_object_del(g_genlist);
		g_genlist = NULL;
	}

	g_genlist = elm_genlist_add(ug_data->navi_frame);
	//g_genlist = genlist;
	elm_genlist_mode_set(g_genlist, ELM_LIST_COMPRESS);
	elm_object_style_set(g_genlist, "dialogue");

	lockscreen_options_main_create_genlist(g_genlist, ug_data);
	elm_object_item_part_content_set(ug_data->main_navi_item, "elm.swallow.content", g_genlist);
	evas_object_show(g_genlist);
	ug_data->locktype = new_locktype;

	/* for starter */
	if (new_locktype < SETTING_SCREEN_LOCK_TYPE_OTHER) {
		ret = vconf_set_str(VCONFKEY_SETAPPL_3RD_LOCK_PKG_NAME_STR, LOCKSCREEN_PACKAGE_NAME);
		LOCKOPTIONS_DBG("VCONFKEY_SETAPPL_3RD_LOCK_PKG_NAME_STR set %s [%d] ", LOCKSCREEN_PACKAGE_NAME, ret);
	}
	return;
}

static void _lockscreen_options_register_vconf_change(lockscreen_options_ug_data * ug_data)
{
	LOCKOPTIONS_TRACE_BEGIN;
	int ret = -1;
	ret = vconf_notify_key_changed(VCONFKEY_SETAPPL_SCREEN_LOCK_TYPE_INT,
					_lockscreen_options_locktype_change_cb, ug_data);
	if (ret != 0) {
		LOCKOPTIONS_ERR("Failed to register lockscreen type callback!");
	}
}

void _lockscreen_options_unregister_vconf_change()
{
	LOCKOPTIONS_TRACE_BEGIN;
	int ret = -1;
	ret = vconf_ignore_key_changed(VCONFKEY_SETAPPL_SCREEN_LOCK_TYPE_INT,
					_lockscreen_options_locktype_change_cb);
	if (ret != 0) {
		LOCKOPTIONS_ERR("Failed to unregister lockscreen type callback!");
	}

}
