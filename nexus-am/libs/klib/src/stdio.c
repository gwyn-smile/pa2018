#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

void get_type(va_list* ap, const char type, char* dest) {
	switch(type) {
		case 'c': {
			char c;
			c = (char)va_arg(*ap, int);
			dest[0] = c;
			dest[1] = '\0';
		} break;
							
		case 's': {
			char* s;
			s = va_arg(*ap, char*);
			strncpy(dest, s, strlen(s));
		} break;

		case 'd': {
			int num, d, ct = 0;
			d = (int32_t)va_arg(*ap, int32_t);
			while(d) {
				num = d % 10;
				d = d / 10;
				dest[ct] = num - '0';
				ct++;
			}
			char tmp;
			int i;
			for(i = 0; i <= (ct - 1) / 2; i++) {
				tmp = dest[i];
				_putc(tmp);
				dest[i] = dest[ct - 1 - i];
				dest[ct - 1 - i] = tmp;
			}
			dest[ct] = '\0';
		} break;
		default: assert(0);
	}	
}

int printf(const char *fmt, ...) {
  va_list ap;
	va_start(ap, fmt);
  int i = 0;
	char tran[256];
	while(fmt[i] != '\0') {
		if(fmt[i] != '%') {
			_putc(fmt[i]);
			i++;
			continue;
		}
		i++;
		//only implment one-bit type_code
		get_type(&ap, fmt[i], tran);
		char *p;
		for(p = tran; *p != '\0'; p++) {
			_putc(*p);
		}
		//next
		i++;
	}
	va_end(ap);
	return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  assert(0);
	return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
	va_start(ap, fmt);
  int i = 0;
	char tran[256];
	while(fmt[i] != '\0') {
		if(fmt[i] != '%') {
			tran[0] = fmt[i];
			tran[1] = '\0';
			strcat(out, tran);
			i++;
			continue;
		}
		i++;
		//only implment one-bit type_code
		get_type(&ap, fmt[i], tran);
		strcat(out, tran);
		//next
		i++;
	}
	va_end(ap);
	return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  assert(0);
	return 0;
}

#endif
