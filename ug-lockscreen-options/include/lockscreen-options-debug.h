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


#ifndef __LOCKSCREEN_OPTIONS_DEBUG_H__
#define __LOCKSCREEN_OPTIONS_DEBUG_H__

#include <stdio.h>

#define OPENLOCK_USING_PLATFORM_DEBUG

#ifdef OPENLOCK_USING_PLATFORM_DEBUG
#ifndef LOG_TAG
#define LOG_TAG "LSO"
#endif
#include <dlog.h>

#define LOCKOPTIONS_DBG(fmt, args...)  LOGD("["LOG_TAG"%s:%d:D] "fmt,  __func__, __LINE__, ##args)
#define LOCKOPTIONS_WARN(fmt, args...) LOGW("["LOG_TAG"%s:%d:W] "fmt,  __func__, __LINE__, ##args)
#define LOCKOPTIONS_ERR(fmt, args...)  LOGE("["LOG_TAG"%s:%d:E] "fmt,  __func__, __LINE__, ##args)

#else
#define LOCKOPTIONS_DBG(fmt, args...) do{printf("[LOCKOPTIONS_DBG][%s(%d)] "fmt " \n", __FILE__, __LINE__, ##args);}while(0);
#define LOCKOPTIONS_WARN(fmt, args...) do{printf("[LOCKOPTIONS_WARN][%s(%d)] "fmt " \n", __FILE__, __LINE__, ##args);}while(0);
#define LOCKOPTIONS_ERR(fmt, args...) do{printf("[LOCKOPTIONS_ERR][%s(%d)] "fmt " \n", __FILE__, __LINE__, ##args);}while(0);
#endif				/* LOCKD_USING_PLATFORM_DEBUG */

#define LOCKOPTIONS_TRACE_BEGIN do {\
		{\
			LOGW("ENTER FUNCTION: %s.\n", __FUNCTION__);\
		}\
	}while(0);

#define LOCKOPTIONS_TRACE_END do {\
		{\
			LOGW("EXIT FUNCTION: %s.\n", __FUNCTION__);\
		}\
	}while(0);

#define ret_if(expr) do { \
	if(expr) { \
		LOCKHELP_ERR("(%s) -> %s() return", #expr, __FUNCTION__); \
		return; \
	} \
} while (0)

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#endif				/* __OPENLOCK_SETTING_DEBUG_H__ */
