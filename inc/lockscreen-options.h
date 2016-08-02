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


#ifndef __LOCKSCREEN_OPTIONS_H__
#define __LOCKSCREEN_OPTIONS_H__

#include <Elementary.h>
#include "lockscreen-options-debug.h"

#define PKGNAME "ug-lockscreen-options"

#define _EDJ(o)			elm_layout_edje_get(o)
#define _S(str)			dgettext("sys_string", str)
#ifdef _
#undef _
#endif
#define _(str)			dgettext(PKGNAME, str)
#define dgettext_noop(s)	(s)
#ifdef N_
#undef N_
#endif
#define N_(s)			dgettext_noop(s)
#define _X(x) (x*elm_config_scale_get())

/**
 * @brief An enum with genlist-item style codes.
 */
enum{
	ENUM_LOCKSCREEN_GENLIST_STYLE_1TEXT = 0,
	ENUM_LOCKSCREEN_GENLIST_STYLE_1TEXT1ICON,
	ENUM_LOCKSCREEN_GENLIST_STYLE_1TEXT1ICON_DIVIDER,
	ENUM_LOCKSCREEN_GENLIST_STYLE_2TEXT1ICON_DIVIDER,
	ENUM_LOCKSCREEN_GENLIST_STYLE_HELP,
	ENUM_LOCKSCREEN_GENLIST_STYLE_TITLE,
	ENUM_LOCKSCREEN_GENLIST_STYLE_VARIABLE_HEIGHT,
	ENUM_LOCKSCREEN_GENLIST_STYLE_2TEXT,
	ENUM_LOCKSCREEN_GENLIST_STYLE_2TEXT_EXPANDABLE,
	ENUM_LOCKSCREEN_GENLIST_STYLE_RADIO,
	ENUM_LOCKSCREEN_GENLIST_STYLE_2TEXT1ICON,
	ENUM_LOCKSCREEN_GENLIST_STYLE_MULTILINE_SUB
};

#define LOCKSCREEN_GENLIST_STYLE_1TEXT "dialogue/1text"
#define LOCKSCREEN_GENLIST_STYLE_1TEXT1ICON "dialogue/1text.1icon"
#define LOCKSCREEN_GENLIST_STYLE_1TEXT1ICON_DIVIDER "dialogue/1text.1icon.divider"
#define LOCKSCREEN_GENLIST_STYLE_2TEXT1ICON_DIVIDER "dialogue/2text.1icon.divider"
#define LOCKSCREEN_GENLIST_STYLE_TITLE "dialogue/grouptitle"
#define LOCKSCREEN_GENLIST_STYLE_2TEXT "dialogue/2text"
#define LOCKSCREEN_GENLIST_STYLE_2TEXT_EXPANDABLE "dialogue/2text/expandable"

/**
 * @brief Lockscreen settings main data-structure.
 */
typedef struct _lockscreen_options_ug_data {
    /**
     * Code of lockscreen-type:
     * - 0 None
     * - 1 unlock by swipe
     * - 5 unlock with simple password(numeric)
     * - 6 unlock with alphanumeric password.
     * */
	int locktype;

	Evas_Object *win_main;              /**< Main window*/
	Evas_Object *base;                  /**< Main layout */

	Evas_Object *navi_frame;            /**< Naviframe */
	Elm_Object_Item *main_navi_item;    /**< Main naviframe item */
	Evas_Object *app_shortcut_genlist;  /**< A genlist that contains 2 items: Lockscreen type and app-shortcut*/
	char *viewtype;                     // Seems to be unused, need to check deeper.
	Ecore_Timer *timer_locktype;        /**< A handler of timer aimed to do a delayed launch of lick-type selection app built-in into settings-package */
} lockscreen_options_ug_data;

#endif				/* __lockscreen_options_H__ */
