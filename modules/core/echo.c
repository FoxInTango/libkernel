#include "echo.h"

#if defined(WIN32) || defined(_WIN32) || defined(_WIN32_) || defined(WIN64) || defined(_WIN64) || defined(_WIN64_)
#ifdef foxintangoEXPORT
#define foxintangoAPI __declspec(dllexport)
#else
#define foxintangoAPI __declspec(dllimport)
#endif
#elif defined(ANDROID) || defined(_ANDROID_)
#define foxintangoAPI __attribute__ ((visibility("default")))
#elif defined(__linux__)
#define foxintangoAPI __attribute__ ((visibility("default")))
#include <linux/kernel.h>
void echo(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintk(fmt, args);
    va_end(args);
}
#elif defined(__APPLE__) || defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_MAC)
#define foxintangoAPI __attribute__ ((visibility("default")))
#else
#define foxintangoAPI
#endif

