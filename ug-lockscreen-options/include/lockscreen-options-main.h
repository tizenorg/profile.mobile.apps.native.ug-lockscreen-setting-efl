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

#ifndef __LOCKSCREEN_OPTIONS_MAIN_H__
#define __LOCKSCREEN_OPTIONS_MAIN_H__

#include "lockscreen-options.h"

void lockscreen_options_main_create_view(lockscreen_options_ug_data * ug_data);
void lockscreen_options_main_update_view();
void _lockscreen_options_unregister_vconf_change();

#endif				/* __LOCKSCREEN_OPTIONS_MAIN_H__ */
