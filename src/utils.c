#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


void p_exit(const char *s, ...)
{
  va_list ap;

  va_start(ap, s);
  vfprintf(stderr, s, ap);
  va_end(ap);

  exit(1);
}
