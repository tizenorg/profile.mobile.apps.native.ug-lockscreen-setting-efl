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


#ifndef __LOCKSCREEN_OPTIONS_UTIL_H__
#define __LOCKSCREEN_OPTIONS_UTIL_H__

#include <Elementary.h>
#include <efl_extension.h>
#include <app_control.h>
#include "lockscreen-options.h"

#define BUF_SIZE                    256

typedef struct {
	int glStyle;
	int stringId;
	Evas_Object *check;
	void (*func) (void *data, Evas_Object * obj, void *event_info);
	void *data;
	Elm_Object_Item *item;
} lockscreen_menu_item_info;

enum {
	IDS_COM_SK_OK = 0,
	IDS_COM_SK_CANCEL,
	IDS_LOCKSCREEN_OPTIONS_SNS,
	IDS_LOCKSCREEN_OPTIONS_SYSTEM_STRING_MAX,
};

enum {
	IDS_LOCKSCREEN_OPTIONS_LOCK_SCREEN_SETTINGS =
	    IDS_LOCKSCREEN_OPTIONS_SYSTEM_STRING_MAX,
	IDS_LOCKSCREEN_ACHEADER2_PERSONAL_MESSAGE,
	IDS_LOCKSCREEN_ACHEADER2_LOCK_SCREEN_THEMES,
	IDS_LOCKSCREEN_ACHEADER1_LOCK_SCREEN_SETTINGS,
	IDS_LOCKSCREEN_ACHEADER1_SET_SHORTCUTS,
	IDS_LOCKSCREEN_OPTIONS_LOCK_SCREEN_TYPE,
	IDS_LOCKSCREEN_OPTIONS_LOCK_SCREEN_WALLPAPER,
	IDS_LOCKSCREEN_OPTIONS_SHORTCUTS,
	IDS_LOCKSCREEN_OPTIONS_SHORTCUTS_HELP,
	IDS_LOCKSCREEN_OPTIONS_SHORTCUTS_SETTINGS,
	IDS_LOCKSCREEN_OPTIONS_SHORTCUTS_SETTINGS_HELP,
	/* stytle start */
	IDS_LOCKSCREEN_OPTIONS_LOCK_SCREEN_STYTLE_TITLE,
	IDS_LOCKSCREEN_OPTIONS_LOCK_SCREEN_STYTLE_CONTENT,
	IDS_LOCKSCREEN_OPTIONS_LOCK_SCREEN_STYTLE_HELP,
	IDS_LOCKSCREEN_OPTIONS_LOCK_SCREEN_STYTLE_HELP_1,
	/* stytle end */
	IDS_LOCKSCREEN_OPTIONS_LOCKSCREEN_BRIEFING,
	IDS_LOCKSCREEN_OPTIONS_LOCKSCREEN_BRIEFING_SUB_TITLE,
	IDS_LOCKSCREEN_OPTIONS_LOCKSCREEN_BRIEFING_HELP,
	IDS_LOCKSCREEN_OPTIONS_LOCK_SCREEN_TITLE,
	IDS_LOCKSCREEN_OPTIONS_LOCK_SCREEN_TITLE_HELP,
	IDS_LOCKSCREEN_OPTIONS_EVENT_NOTIFICATIONS,
	IDS_LOCKSCREEN_OPTIONS_EVENT_NOTIFICATIONS_HELP,
	IDS_LOCKSCREEN_OPTIONS_CONTEXTAWARE_NOTI,
	IDS_LOCKSCREEN_OPTIONS_CONTEXTAWARE_NOTI_HELP,
#if USE_CAMERA
	IDS_LOCKSCREEN_OPTIONS_CAMERA_QUICK_ACCESS,
	IDS_LOCKSCREEN_OPTIONS_CAMERA_QUICK_ACCESS_HELP,
#endif
	IDS_LOCKSCREEN_OPTIONS_CLOCK,
	IDS_LOCKSCREEN_OPTIONS_DUAL_CLOCK,
	IDS_LOCKSCREEN_OPTIONS_DUAL_CLOCK_HELP,
	IDS_LOCKSCREEN_OPTIONS_DUAL_CLOCK_CONTENT,
	IDS_LOCKSCREEN_OPTIONS_WAKE_UP,
	IDS_LOCKSCREEN_OPTIONS_WAKE_UP_HELP,
	IDS_LOCKSCREEN_OPTIONS_WEATHER,
	IDS_LOCKSCREEN_OPTIONS_HELPTEXT,
	IDS_LOCKSCREEN_OPTIONS_HELPTEXT_HELP,
	IDS_LOCKSCREEN_OPTIONS_SET_SHORTCUTS,
	IDS_LOCKSCREEN_OPTIONS_EDIT_SHORTCUTS,
	IDS_LOCKSCREEN_OPTIONS_SET_SHORTCUTS_ON_LOCKSCREEN,
	IDS_LOCKSCREEN_OPTIONS_TAP_SHORTCUTS,
	IDS_LOCKSCREEN_OPTIONS_DRAG_DROP_SHORTCUTS,
	IDS_LOCKSCREEN_OPTIONS_SELECT_APPLICATIONS,
	IDS_LOCKSCREEN_OPTIONS_LOCK_SCREEN_TITLE_GUIDE_TEXT,
	IDS_LOCKSCREEN_OPTIONS_LOCK_SCREEN_THEME,
	IDS_LOCKSCREEN_OPTIONS_SET_LOCKSTYLE,
	IDS_LOCKSCREEN_OPTIONS_PREVIEW,
	IDS_LOCKSCREEN_OPTIONS_APPLY,
	IDS_LOCKSCREEN_OPTIONS_LOCK_SCREEN_BRIEFING,
	IDS_LOCKSCREEN_OPTIONS_BRIEFING_ITEMS,
	IDS_LOCKSCREEN_OPTIONS_MUSIC_CONTROLLER,
	IDS_LOCKSCREEN_OPTIONS_MISSED_EVENTS,
	IDS_LOCKSCREEN_OPTIONS_NEWS,
	IDS_LOCKSCREEN_OPTIONS_SCHEDULE,
	IDS_LOCKSCREEN_OPTIONS_CLEAR,
	IDS_LOCKSCREEN_OPTIONS_CLOSEUP,
	IDS_LOCKSCREEN_OPTIONS_BOOKCOVER,
	IDS_LOCKSCREEN_OPTIONS_SPLENDID,
	IDS_LOCKSCREEN_OPTIONS_PEACEFUL,
	IDS_LOCKSCREEN_OPTIONS_FUN,
	IDS_LOCKSCREEN_OPTIONS_HUMAN,
	IDS_LOCKSCREEN_OPTIONS_WEATHER_THEME,
	IDS_ST_BODY_STYLE,
	IDS_LOCKSCREEN_OPTIONS_STYLE_NONE,
	IDS_LOCKSCREEN_OPTIONS_STYLE_SWIPE,
	IDS_LOCKSCREEN_OPTIONS_STYLE_MOTION,
	IDS_LOCKSCREEN_OPTIONS_STYLE_VOICE,
	IDS_LOCKSCREEN_OPTIONS_STYLE_SIMPLE_PASSWORD,
	IDS_LOCKSCREEN_OPTIONS_STYLE_PASSWORD,
	IDS_LOCKSCREEN_OPTIONS_WALLPAPER_CHANGED,
	IDS_LOCKSCREEN_OPTIONS_SET_WAKE_UP,
	IDS_LOCKSCREEN_OPTIONS_EDIT_PERSONAL_MESSAGE,
	IDS_LOCKSCREEN_OPTIONS_EDIT,
	IDS_LOCKSCREEN_OPTIONS_EDIT_HELP,
	IDS_LOCKSCREEN_OPTIONS_UNLOCK_EFFECT,
	IDS_LOCKSCREEN_OPTIONS_CONFETTI_M_EFFECT,
	IDS_LOCKSCREEN_OPTIONS_GEOMETRIC_M_EFFECT,
	IDS_LOCKSCREEN_OPTIONS_STARDUST_M_EFFECT,
	IDS_LOCKSCREEN_OPTIONS_GLOWWORM_M_EFFECT,
	IDS_LOCKSCREEN_OPTIONS_EDIT_TITLE,
	IDS_LOCKSCREEN_OPTIONS_FESTIVAL_WALLPAPER,
	IDS_LOCKSCREEN_OPTIONS_WEATHER_EFFECT,
	IDS_LOCKSCREEN_OPTIONS_LOCK_SCREEN_WEATHEREFFECT_HELP,
	IDS_LOCKSCREEN_OPTIONS_STYLE_FINGERPRINT,
	IDS_LOCKSCREEN_OPTIONS_STYLE_AUTOLOCK,
	IDS_LOCKSCREEN_OPTIONS_APP_SHORTCUT,
	IDS_LOCKSCREEN_OPTIONS_SELECT_WHICH_APP_SHORTCUT,
	IDS_LOCKSCREEN_OPTIONS_ADD_INFORMATION,
	IDS_LOCKSCREEN_OPTIONS_ADD_INFORMATION_HELP,
	IDS_LOCKSCREEN_OPTIONS_ADDINF_WEATHER,
	IDS_LOCKSCREEN_OPTIONS_PEDOMETER,
	IDS_LOCKSCREEN_OPTIONS_ADDINF_HELP,
	IDS_LOCKSCREEN_OPTIONS_WEATHER_EFFECT_HELP,
	IDS_LOCKSCREEN_OPTIONS_HELP_SHORTCUT,
	IDS_LOCKSCREEN_OPTIONS_HELP_SHORTCUT_HELP,
	IDS_LOCKSCREEN_OPTIONS_FESTIVAL_WALLPAPER_HELP,
	IDS_LOCKSCREEN_OPTIONS_SWIPE_PW,
	IDS_LOCKSCREEN_OPTIONS_SWIPE_PW_HELP,
	IDS_LOCKSCREEN_OPTIONS_APP_STRING_MAX,	/* 55 */
};

typedef enum {
	LOCKSCREEN_READER_OBJ_TYPE_ELM_OBJECT,
	LOCKSCREEN_READER_OBJ_TYPE_EDJ_OBJECT,
} lockscreen_object_type_e;

Evas_Object *lockscreen_options_util_create_navigation(Evas_Object * parent);
Evas_Object *lockscreen_options_util_create_layout(Evas_Object * parent,
						   const char *file,
						   const char *group);
char *lockscreen_optoins_get_string(int id);
void _lockscreen_options_submenu_gl_item(Elm_Gen_Item_Class * item);
void remove_unused_string(char *text, char *unused_string);
void replace_unused_string_with_char(char *text, char *unused_string, char c);

int get_vconf_screenreader(void);
void *lockscreen_options_tts_reader_object_get(void *obj, lockscreen_object_type_e type, const char *part, void *parent);
void *lockscreen_options_tts_get_focus_object(void *parent);
void lockscreen_options_set_tts_info(Evas_Object* obj, const char* label,
			  const char* traits, const char *state,
			  const char* guide);

void launch_ug(char* ug_name, app_control_h svc_mt_ug, void *data);


#endif				/* __OPENLOCK_SETTING_UTIL_H__ */
