#include "klib.h"
#include <stdarg.h>

#ifndef __ISA_NATIVE__

int printf(const char *fmt, ...) {
  assert(0);
	return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  assert(0);
	return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  assert(0);
	return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  assert(0);
	return 0;
}

#endif
