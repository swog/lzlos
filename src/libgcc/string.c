#include <string.h>

// String concatenate simple
char* strncat(char* dst, const char* src, size_t num) {
	char* ptr = dst;

	while (*ptr) {
		ptr++;
	}

	for (size_t i = 0; i < num; i++) {
		*ptr = src[i];
		ptr++;
	}

	*ptr = '\0';	

	return dst;
}

char* strcat(char* dst, const char* src) {
	char* ptr = dst;

	while (*ptr) {
		ptr++;
	}

	do {
		*ptr++ = *src++;
	} while (*src);

	return dst;
}

char* strchr(const char* str, int c) {
	char* ptr = (char*)str;
	
	while (*ptr) {
		if (*ptr == c) {
			return ptr;
		}
	}
	
	return NULL;
}

int isxdigit(int ch) {
	return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
}

int isspace(int ch) {
	return !(ch > ' ' && ch <= '~');
}

int islower(int ch) {
	return ch >= 'a';
}

int isupper(int ch) {
	return ch < 'a';
}

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
	char *ptr = dst;

	if (*src) {
		do {
			*ptr++ = *src++;
		} while (*src);
	}

	return dst;
}

char *strncpy(char *dst, const char *src, size_t num) {
	char* ptr = dst;

	if (*src && num) {
		do {
			*ptr++ = *src++;
			num--;
		} while (*src && num);
	}

	return dst;
}







