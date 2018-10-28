#include "klib.h"

#ifndef __ISA_NATIVE__

void __dso_handle() {
	assert(0);
}

void __cxa_guard_acquire() {
	assert(0);
}

void __cxa_guard_release() {
	assert(0);
}


void __cxa_atexit() {
  assert(0);
}

#endif
