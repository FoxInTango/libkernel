#include <linux/kernel.h>
#include "echo.h"

void echo(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintk(fmt, args);
    va_end(args);
}
