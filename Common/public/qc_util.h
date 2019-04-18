//
//  qc_util.h
//  qqdz
//
//  Created by ldm on 2017/4/17.
//  Copyright © 2017年 ldm. All rights reserved.
//

#ifndef qc_util_h
#define qc_util_h

#include "qc_types.h"

// Which platform we are on?
#if _MSC_VER
#define UNITY_WIN 1
#elif defined(__APPLE__)
#if defined(__arm__)
#define UNITY_IPHONE 1
#else
#define UNITY_OSX 1
#endif
#elif defined(__linux__)
#define UNITY_LINUX 1
#elif defined(UNITY_METRO) || defined(UNITY_ANDROID)
// these are defined externally
#else
#error "Unknown platform!"
#endif

// Attribute to make function be exported from a plugin
#if UNITY_METRO
#define EXPORT_API extern "C"  __declspec(dllexport) __stdcall
#elif UNITY_WIN
#define EXPORT_API extern "C"  __declspec(dllexport)
#else
#define EXPORT_API extern "C" 
#endif

// define log function
#if defined(__linux__)
#include <android/log.h>
#define _log(...) do {\
((void)__android_log_print(ANDROID_LOG_ERROR, "libqc", "[%-6d] ", iclock())); \
((void)__android_log_print(ANDROID_LOG_ERROR, "libqc", __VA_ARGS__));\
((void)__android_log_print(ANDROID_LOG_ERROR, "libqc", "\n")); \
}while(0)
#elif defined(__APPLE__)
#define _log(format, args...) do{\
fprintf(stdout, "[%-6d] ", iclock());\
fprintf(stdout, format, ##args);\
fprintf(stdout, "\n");\
}while(0)
#else 
#define _log(format, ...) do{\
fprintf(stdout, "[%-6d] ", iclock());\
fprintf(stdout, format, __VA_ARGS__);\
fprintf(stdout, "\n");\
}while(0)
#endif

#endif