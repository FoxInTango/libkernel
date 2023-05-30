/*
 * Kernel C++ support
 */

#include "kmm.h"
#include "echo.h"

#include <linux/mm.h>

#include <cstddef>
void *operator new(size_t sz) throw ()
{
    return kcmalloc(sz);
}

void *operator new[](size_t sz) throw ()
{
    return kcmalloc(sz);
}

void operator delete(void *p)
{
    kcfree(p);
}

void operator delete[](void *p)
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
