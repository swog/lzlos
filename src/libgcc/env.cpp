#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Hash function for env hash map.
// djb2
static size_t env_hash(const char* name) {
	size_t hash = 5381;

	for (int i = 0; name[i]; i++) {
		hash = ((hash << 5) + hash) + name[i];
	}

	return hash;
}

typedef struct libc_env_s {
	const char* 		name;
	size_t			size;
	char* 			value;
	struct libc_env_s* 	next;
} libc_env_t;

#define LIBC_ENV_HASHSIZE 31

// Use prime number
static libc_env_t* libc_env_ht[LIBC_ENV_HASHSIZE] = {NULL};

static libc_env_t* env_find(const char* name, size_t hash) {
	libc_env_t* ptr = libc_env_ht[hash % LIBC_ENV_HASHSIZE];

	if (!ptr) {
		return NULL;
	}

	while (ptr) {
		if (!strcmp(name, ptr->name)) {
			break;
		}

		ptr = ptr->next;
	}

	return ptr;
}

// Set env variable
int setenv(const char* name, const char* value, int overwrite) {
	if (!name || !*name) {
		// errno = x;
		return -1;
	}

	size_t hash = env_hash(name);
	libc_env_t* ptr = env_find(name, hash);
	size_t value_size = strlen(value)+1;

	if (ptr) {
		if (!overwrite) {
			// errno = x;
			return -1;
		}

		ptr->value = (char*)realloc(ptr->value, value_size);
	
		if (!ptr->value) {
			// errno = x;
			return -1;
		}

		ptr->size = value_size;

		strcpy(ptr->value, value);
	}
	// Not found, allocate new
	else {
		ptr = (libc_env_t*)malloc(sizeof(libc_env_t));

		if (!ptr) {
			// errno = x;
			return -1;
		}

		ptr->name = name;
		ptr->value = (char*)malloc(value_size);
		ptr->size = value_size;

		strcpy(ptr->value, value);
	}

	return 0;
}

// Get env variable
char* getenv(const char* name) {
	if (!name || !*name) {
		return NULL;
	}

	size_t hash = env_hash(name);
	libc_env_t* ptr = env_find(name, hash);

	if (!ptr) {
		return NULL;
	}

	return ptr->value;
}

















