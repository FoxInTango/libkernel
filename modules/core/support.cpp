/*
 * Kernel C++ support
 */

#include "kmm.h"
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
#include <cstddef>
void* operator new(size_t sz) throw ()
{
    return kcmalloc((unsigned int)sz);
}

void* operator new[](size_t sz) throw ()
    {
        return kcmalloc((unsigned int)sz);
    }

    void operator delete(void* p)
    {
        kcfree(p);
    }

    void operator delete[](void* p)
        {
            kcfree(p);
        }

        void terminate()
        {
            echo("terminate requested\n");
        }

        extern "C" void __cxa_pure_virtual()
        {
            echo("cxa_pure_virtual error handler\n");
        }
#elif defined(__APPLE__) || defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_MAC)
#define foxintangoAPI __attribute__ ((visibility("default")))
#else
#define foxintangoAPI
#endif

