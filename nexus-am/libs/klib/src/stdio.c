#include "klib.h"
#include <stdarg.h>

#ifndef __ISA_NATIVE__

void str_tra(char* oper, size_t len) {
	char ret[256];
	size_t i;
	for(i = 0;i <= len - 1;i++) {
		ret[i] = *(oper + len - i - 1);
	}
	ret[i] = '\0';
	strncpy(oper, ret, len);
}

int printf(const char *fmt, ...) {
  assert(0);
	return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  assert(0);
	return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
	int32_t d;
	char* s;
	char c;
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
		switch(fmt[i]) {
			case 'c': {
				c = (char)va_arg(ap, int);
				tran[0] = c;
				tran[1] = '\0';
				strcat(out, tran);
			} break;

			case 's': {
				s = va_arg(ap, char*);
				strcat(out, s);
			} break;

			case 'd': {
				int num, ct = 0;
				d = (int32_t)va_arg(ap, int32_t);
				while(d) {
					num = d % 10;
					d = d / 10;
					tran[ct] = num - '0';
					ct++;
				}
				tran[ct] = '\0';
				str_tra(tran, strlen(tran));
			} break;
		}
	}
	va_end(ap);
	return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  assert(0);
	return 0;
}

#endif
