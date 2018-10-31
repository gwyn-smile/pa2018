#include <am.h>
#include <x86.h>
#include <amdev.h>
#include <klib.h>

size_t input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      _KbdReg *kbd = (_KbdReg *)buf;
			int key_tmp = read_key();
			if(key_tmp & 0x8000) {
				kbd->keydown = 0;
				kbd->keycode = key_tmp;
			}
			else {
				kbd->keydown = 1;
				kbd->keycode = _KEY_NONE;
			}
      return sizeof(_KbdReg);
    }
  }
  return 0;
}
