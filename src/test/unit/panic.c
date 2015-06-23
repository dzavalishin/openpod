
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>



void pod_panic(const char *fmt, ...)
{

    printf("Panic: ");
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);

    exit(33);
}

