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

#include "app-shortcut.h"
#include "lockscreen-options-debug.h"
#include <stdbool.h>
#include <package_manager.h>
#include <package_info.h>
#include <app_manager.h>
#include <app_common.h>

#define TEXT_SIZE 32
#define ICON_SIZE 82
#define TEXT_MAX_LENGTH 1024

const char *chosen_app = NULL;

static lockscreen_options_ug_data *ug_data = NULL;
static bool show = false;

static char *gl_option_text_get_cb(void *data, Evas_Object *obj, const char *part);
static char* gl_text_get_cb(void *data, Evas_Object *obj, const char *part);
static Evas_Object* gl_content_get_cb(void *data, Evas_Object *obj, const char *part);
static Evas_Object* gl_option_content_get_cb(void *data, Evas_Object *obj, const char *part);
static void append_gl_app_shortcut_item(char *style, char *ugName);
static void append_gl_item_list(Eina_List* list, char *style);
static void app_shortcut_create_view();
static void app_shortcut_check_changed_cb(void *data, Evas_Object *obj, void *event_info);
static void cancel_button_cb(void *data, Evas_Object *obj, void *event_info);
static void done_button_cb(void *data, Evas_Object *obj, void *event_info);
static void gl_selected_cb(void *data, Evas_Object *obj EINA_UNUSED, void *event_info);
static void gl_radio_sel_cb(void *data, Evas_Object *obj, void *event_info);
static bool _app_manager_app_info_cb(app_info_h app_info, void *user_data);
static int apps_sort_cb(const void *d1, const void *d2);
static Eina_List *app_manager_all_apps_get();

void fill_app_shortcut_list();
void gl_del_cb(void *data, Evas_Object *obj EINA_UNUSED);
bool get_app_shortcut_state();
Evas_Object* create_icon(Evas_Object *parent, char *icon);

struct index_item
{
        int index;
        app_info_h app_info;
};

static char *gl_option_text_get_cb(void *data, Evas_Object *obj, const char *part)
{
    if(!data)
        return NULL;

    char buf[TEXT_MAX_LENGTH] = { 0, };

    if (!strcmp(data, "app-shortcut") && !strcmp(part, "elm.text"))
        if(get_app_shortcut_state())
            return strdup("App shortcut"); // TODO: change when IDS will be created
        else
            return strdup("Lock screen shortcut"); // TODO: change when IDS will be created
    else if (!strcmp(data, "help-text-item") && !strcmp("elm.text.multiline", part))
    {
        if (get_app_shortcut_state())
            snprintf(buf, sizeof(buf), "<font_size=%d>%s</font_size>", TEXT_SIZE, "Select which app shortcut you want to use on the lock screen");//TODO: add IDS
        else
            snprintf(buf, sizeof(buf), "<font_size=%d>%s</font_size>", TEXT_SIZE, "Use app shortcut on the lock screen");//TODO: add IDS

        return strdup(buf);
    }

    return NULL;
}

static void app_shortcut_check_changed_cb(void *data, Evas_Object *obj, void *event_info)
{
    LOCKOPTIONS_TRACE_BEGIN;

    show = elm_check_state_get(obj);
    if(!show)
    {
        chosen_app = NULL;
    }

    elm_genlist_clear(ug_data->app_shortcut_genlist);
    fill_app_shortcut_list();

    LOCKOPTIONS_DBG("app_shortcut check value = %s", show == false ? "FALSE":"TRUE");
}

bool get_app_shortcut_state()
{
    // TODO: implement when API will be available
    return show;
}

static Evas_Object* gl_option_content_get_cb(void *data, Evas_Object *obj, const char *part)
{
    if(!data)
        return NULL;

    if(!strcmp(data, "app-shortcut") && !strcmp(part, "elm.swallow.end"))
    {
        Evas_Object *check = NULL;
        check = elm_check_add(obj);
        elm_object_style_set(check, "on&off");
        elm_check_state_set(check, get_app_shortcut_state());
        evas_object_show(check);
        evas_object_pass_events_set(check, 1);
        evas_object_smart_callback_add(check, "changed", app_shortcut_check_changed_cb, NULL);
        evas_object_propagate_events_set(check, 0);
        return check;
    }
    return NULL;
}

static void gl_selected_cb(void *data, Evas_Object *obj EINA_UNUSED, void *event_info)
{
    LOCKOPTIONS_TRACE_BEGIN;
    Elm_Object_Item *it = (Elm_Object_Item*)event_info;
    elm_genlist_item_selected_set(it, EINA_FALSE);

    Evas_Object *check = elm_object_item_part_content_get(it, "elm.swallow.end");
    if(check)
    {
        elm_check_state_set(check, !elm_check_state_get(check));
        evas_object_smart_callback_call(check, "changed", NULL);
    }
}

void gl_del_cb(void *data, Evas_Object *obj EINA_UNUSED)
{
    if(data != NULL)
    {
        free(data);
        data = NULL;
    }
}

static void append_gl_app_shortcut_item(char *style, char *ugName)
{
    LOCKOPTIONS_TRACE_BEGIN;

    Elm_Genlist_Item_Class *itc = elm_genlist_item_class_new();
    ret_if(!itc);

    itc->item_style = style;
    itc->func.text_get = gl_option_text_get_cb;
    itc->func.content_get = gl_option_content_get_cb;
    itc->func.del = NULL;

    elm_genlist_item_append(ug_data->app_shortcut_genlist,      /* genlist object */
                            itc,                                /* item class */
                            ugName,                             /* item class user data */
                            NULL,                               /* parent item */
                            ELM_GENLIST_ITEM_NONE,              /* item type */
                            gl_selected_cb,                     /* select smart callback */
                            ug_data);                           /* smart callback user data */

    elm_genlist_item_class_free(itc);
}

Evas_Object* create_icon(Evas_Object *parent, char *icon)
{
    Evas_Object *img = elm_image_add(parent);

    if(access(icon, F_OK) == -1) {
        char *res_path = app_get_resource_path();
        if (res_path)
        {
            char buf[PATH_MAX] = {0, };
            snprintf(buf, PATH_MAX, "%s%s", res_path, "images/unknown.png");
            icon = strdup(buf);
            free(res_path);
        }
    }

    elm_image_file_set(img, icon, NULL);
    evas_object_size_hint_min_set(img, ICON_SIZE, ICON_SIZE);
    return img;

}

static char* gl_text_get_cb(void *data, Evas_Object *obj, const char *part)
{
    if(data == NULL)
        return NULL;

    struct index_item *item_index = data;
    app_info_h item = item_index->app_info;

    if(!strcmp(part, "elm.text"))
    {
        char *label = NULL;
        app_info_get_label(item, &label);
        return label;
    }

    return NULL;
}

static Evas_Object* gl_content_get_cb(void *data, Evas_Object *obj, const char *part)
{
    if(data == NULL)
        return NULL;

    struct index_item *item_index = data;
    app_info_h item = item_index->app_info;

    if(!strcmp(part, "elm.swallow.icon") || !strcmp(part, "elm.icon.left"))
    {
        char *icon_path = NULL;
        app_info_get_icon(item, &icon_path);

        Evas_Object *icon = create_icon(obj, icon_path);
        evas_object_show(icon);
        return icon;
    }

    int count = item_index->index;
    if (!strcmp(part, "elm.swallow.end"))
    {
        Elm_Object_Item *it = elm_genlist_nth_item_get(obj, count);
        Evas_Object *radio;
        Evas_Object *radio_main = evas_object_data_get(obj, "radio");
        radio = elm_radio_add(obj);
        elm_radio_group_add(radio, radio_main);
        elm_radio_state_value_set(radio, count + 1);
        evas_object_size_hint_weight_set(radio, EVAS_HINT_EXPAND,EVAS_HINT_EXPAND);
        evas_object_size_hint_align_set(radio, EVAS_HINT_FILL, EVAS_HINT_FILL);
        evas_object_propagate_events_set(radio, EINA_FALSE);
        elm_atspi_accessible_relationship_append(it, ELM_ATSPI_RELATION_DESCRIBED_BY, radio);
        elm_atspi_accessible_relationship_append(radio, ELM_ATSPI_RELATION_CONTROLLED_BY, it);
        return radio;
    }

    return NULL;
}

static void gl_radio_sel_cb(void *data, Evas_Object *obj, void *event_info)
{
    LOCKOPTIONS_TRACE_BEGIN;
    Elm_Object_Item *it = event_info;
    int index = (int)data;

    Evas_Object *radio;

    elm_genlist_item_selected_set(it, EINA_FALSE);
    radio = elm_object_item_part_content_get(it, "elm.swallow.end");
    elm_radio_value_set(radio, index + 1);

    chosen_app = elm_object_item_part_text_get(it, "elm.text");
    LOCKOPTIONS_DBG("CHOSEN_APP = %s", chosen_app);
}

static void append_gl_item_list(Eina_List* list, char *style)
{
    LOCKOPTIONS_TRACE_BEGIN;
    Elm_Genlist_Item_Class *itc = elm_genlist_item_class_new();
    ret_if(!itc);

    int count = 0;

    Evas_Object *radio = elm_radio_add(ug_data->app_shortcut_genlist);
    elm_radio_state_value_set(radio, 0);
    elm_radio_value_set(radio, 0);
    evas_object_data_set(ug_data->app_shortcut_genlist, "radio", radio);

    itc->item_style = style;
    itc->func.text_get = gl_text_get_cb;
    itc->func.content_get = gl_content_get_cb;
    itc->func.del = gl_del_cb;

    while(list)
    {
        struct index_item *item = calloc(1, sizeof(struct index_item));
        item->app_info = eina_list_data_get(list);
        item->index = count;
        elm_genlist_item_append(ug_data->app_shortcut_genlist,  /* genlist object */
                                itc,                            /* item class */
                                item,                           /* item class user data */
                                NULL,                           /* parent item */
                                ELM_GENLIST_ITEM_NONE,          /* item type */
                                gl_radio_sel_cb,                /* select smart callback */
                                (void *)count);                 /* smart callback user data */
        list = eina_list_next(list);
        ++count;
    }
    elm_genlist_item_class_free(itc);
}

static void app_shortcut_create_view()
{
    LOCKOPTIONS_TRACE_BEGIN;

    Evas_Object *genlist = NULL;
    Evas_Object *cancel_btn = NULL;
    Evas_Object *done_btn = NULL;

    Elm_Object_Item *navi_item = elm_naviframe_item_push(ug_data->navi_bar, "App shortcut", NULL, NULL, NULL, NULL); // TODO: when IDS will be add
    elm_object_item_domain_text_translatable_set(navi_item, PKGNAME, EINA_TRUE);

    /* Create genlist */
    genlist = elm_genlist_add(ug_data->navi_bar);
    elm_genlist_mode_set(genlist, ELM_LIST_SCROLL);
    elm_genlist_mode_set(genlist, ELM_LIST_COMPRESS);
    elm_object_style_set(genlist, "dialogue");
    evas_object_size_hint_weight_set(genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_genlist_block_count_set(genlist, 8);

    /* Fill list */
    ug_data->app_shortcut_genlist = genlist;
    fill_app_shortcut_list();
    elm_object_item_part_content_set(navi_item, "elm.swallow.content", ug_data->app_shortcut_genlist);

    /* Title Cancel Button */
    cancel_btn = elm_button_add(ug_data->navi_bar);
    elm_object_style_set(cancel_btn, "naviframe/title_left");
    evas_object_smart_callback_add(cancel_btn, "clicked", cancel_button_cb, ug_data->navi_bar);
    elm_object_text_set(cancel_btn, dgettext(PKGNAME, "IDS_ST_BUTTON_CANCEL"));
    elm_object_item_part_content_set(navi_item, "title_left_btn", cancel_btn);

    /* Title Done Button */
    done_btn = elm_button_add(ug_data->navi_bar);
    elm_object_style_set(done_btn, "naviframe/title_right");
    evas_object_smart_callback_add(done_btn, "clicked", done_button_cb, ug_data->navi_bar);
    elm_object_text_set(done_btn, "DONE"); // TODO: when IDS of Done will be add
    elm_object_item_part_content_set(navi_item, "title_right_btn", done_btn);
}

static void cancel_button_cb(void *data, Evas_Object *obj, void *event_info)
{
    LOCKOPTIONS_TRACE_BEGIN;
    elm_naviframe_item_pop(data);
}
static void done_button_cb(void *data, Evas_Object *obj, void *event_info)
{
    LOCKOPTIONS_TRACE_BEGIN;
    elm_naviframe_item_pop(data);
    lockscreen_options_main_update_view();
}

static bool _app_manager_app_info_cb(app_info_h app_info, void *user_data)
{
   bool nodisplay;
   app_info_is_nodisplay(app_info, &nodisplay);
   if (!nodisplay)
   {
       Eina_List **app_list = user_data;
       app_info_h clone = NULL;
       app_info_clone(&clone, app_info);
       *app_list = eina_list_append(*app_list, clone);
   }

   return true;
}

static int apps_sort_cb(const void *d1, const void *d2)
{
    app_info_h tmp1 = (app_info_h)d1;
    app_info_h tmp2 = (app_info_h)d2;

    char *label1 = NULL;
    char *label2 = NULL;
    app_info_get_label(tmp1, &label1);
    app_info_get_label(tmp2, &label2);

    if (!tmp1 || !label1) return 1;
    else if (!tmp2 || !label2) return -1;

    return strcmp(label1, label2);
}

static Eina_List *app_manager_all_apps_get()
{
    LOCKOPTIONS_TRACE_BEGIN;
    Eina_List *app_list = NULL;

    if(get_app_shortcut_state())
    {
        app_manager_foreach_app_info(_app_manager_app_info_cb, &app_list);
        app_list = eina_list_sort(app_list, eina_list_count(app_list), apps_sort_cb);
    }

    return app_list;
}

void fill_app_shortcut_list()
{
    LOCKOPTIONS_TRACE_BEGIN;
    append_gl_app_shortcut_item("type1", "app-shortcut");
    append_gl_app_shortcut_item("multiline", "help-text-item");
    append_gl_item_list(app_manager_all_apps_get(), "default");
}

void create_app_shortcut_ug(void *data)
{
    LOCKOPTIONS_TRACE_BEGIN;
    if(data == NULL)
    {
        LOCKOPTIONS_ERR("The data (ug_data)is NULL.");
        return;
    }
    ug_data = data;
    app_shortcut_create_view();
}
