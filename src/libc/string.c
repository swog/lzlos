#include <string.h>

size_t strlen(const char* str) {
	size_t i;
	for (i = 0; str[i]; i++);
	return i;
}

void *memset(void *dst, int value, size_t num) {
	unsigned char *ptr = (unsigned char*)dst;
	
	for (size_t i = 0; i < num; i++) {
		ptr[i] = (unsigned char)value;
	}

	return dst;
}

void *memcpy(void *dst, const void *src, size_t num) {
	unsigned char *ptr = (unsigned char*)dst;
	const unsigned char *srcp = (unsigned char*)src;

	for (size_t i = 0; i < num; i++) {
		ptr[i] = srcp[i];
	}

	return dst;
}

int strcmp(const char* str1, const char* str2) {
	while (*str1) {
		if (*str1 != *str2) {
			return *str1 - *str2;
		}
		str1++;
	}

	return 0;
}

int memcmp(const void* ptr1, const void* ptr2, size_t num) {
	const unsigned char* str1 = ptr1;
	const unsigned char* str2 = ptr2;

	for (size_t i = 0; i < num; i++) {
		if (str1[i] != str2[i]) {
			return str1[i] - str2[i];
		}
	}

	return 0;
}

char *strcpy(char *dst, const char *src) {
	char *dstp = dst;

	while (*src) {
		*dstp = *src;
		dstp++;
		src++;
	}

	*dstp = 0;

	return dst;
}

char *strncpy(char *dst, const char *src, size_t num) {
	for (size_t i = 0; i < num; i++) {
		dst[i] = *src;
		
		if (*src) {
			src++;
		}
	}

	return dst;
}







