#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
	assert(NULL != s);
	size_t count = 0;
	while(*s != '\0') {
		count++;
		s++;
	}
	return count;
}

char *strcpy(char* dst,const char* src) {
	assert(NULL != dst && NULL != src);
	size_t i;
	for(i = 0; src[i] != '\0'; i++)
		dst[i] = src[i];
	dst[i] = '\0';
	return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
	assert(NULL != dst && NULL != src);
	size_t i;
	for(i = 0; i < n && src[i] != '\0'; i++)
		dst[i] = src[i];
	for( ; i < n; i++)
		dst[i] = '\0';
  return dst;
}

char* strcat(char* dst, const char* src) {
	assert(NULL != dst && NULL != src);
  size_t dst_len = strlen(dst);
	size_t i;
	for(i = 0; src[i] != '\0'; i++)
		dst[dst_len + i] = src[i];
	dst[dst_len + i] = '\0';
	return dst;
}

int strcmp(const char* s1, const char* s2) {
	assert(NULL != s1 && NULL != s2);
  while ((*s1) && (*s1 == *s2))	{
		s1++;
		s2++;
	}
	if (*(unsigned char*)s1 > *(unsigned char*)s2)
		return 1;
	else if (*(unsigned char*)s1 < *(unsigned char*)s2)
		return -1;
	else
		return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
	assert(NULL != s1 && NULL != s2);
  while ((*s1) && (*s1 == *s2) && n)  {
		s1++;
		s2++;
		n--;
	}
	if (*(unsigned char*)s1 > *(unsigned char*)s2)
		return 1;
	else if (*(unsigned char*)s1 < *(unsigned char*)s2)
		return -1;
	else
		return 0;
}

void* memset(void* v,int c,size_t n) {
	assert(NULL != v);
	void* ret = v;
	while(n--) {
		*(char*)v = (char)c;
		v = (char*)v + 1;
	}
  return ret;
}

void* memcpy(void* out, const void* in, size_t n) {
	assert(NULL != out && NULL != in);
	void* ret = out;
	if(out <= in || (char*)out >= (char*)in + n) {
		while(n--) {
			*(char*)out = *(char*)in;
			out = (char*)out + 1;
			in = (char*)in + 1;
		}
	}
	else {
		in = (char*)in + n - 1;
		out = (char*)out + n - 1;
		while(n--) {
			*(char*)out = *(char*)in;
			out = (char*)out - 1;
			in = (char*)in - 1;
		}
	}
  return ret;
}

int memcmp(const void* s1, const void* s2, size_t n){
	assert(NULL != s1 && NULL != s2);
	const unsigned char *su1, *su2;

  for(su1 = s1, su2 = s2; 0 < n; ++su1, ++su2, --n)
		if(*su1 != *su2)
			return ((*su1 < *su2) ? -1 : +1);
	return (0);
}

#endif
