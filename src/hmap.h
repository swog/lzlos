//===============================================
// Hash map.
//===============================================

#pragma once

//===============================================
// Hash map bucket
//===============================================
typedef struct hmap_bucket_s {
	unsigned short			count;
	const void*				key;
	void*					data;
	struct hmap_bucket_s*	next;
	struct hmap_bucket_s*	prev;
} hmap_bucket_t;

// Hash a key
typedef unsigned int hmap_hash_t(const void *key);

//===============================================
// Compare two keys
// 0- Equal
// a- Hash map stored key
// b- Supplied key
//===============================================
typedef int hmap_compare_t(const void *a, const void *b);

//===============================================
// Hash map struct
//===============================================
typedef struct hmap_s {
	unsigned short	count;
	allocator_t*	alloc;
	deallocator_t*	dealloc;
	hmap_hash_t*	hash;
	hmap_compare_t*	compare;
	hmap_bucket_t**	list;
} hmap_t;

// Free map
void hmap_free(hmap_t *map,
	// Free list boolean
	int free_list);
// Initialize hash map
void hmap_init(hmap_t *map, 
	// Number of buckets in the list
	// Recommended to be a prime.
	unsigned short count,
	// Allocate list boolean
	int alloc_list,
	allocator_t *alloc, deallocator_t *dealloc, 
	hmap_hash_t *hash, hmap_compare_t *compare);
// Insert into hash map
void hmap_insert(hmap_t *map, 
	const void *key, void *data);
// Remove from hash map
void hmap_remove(hmap_t *map, const void *key);
// Index the hash map
void* hmap_at(hmap_t *map, const void *key);
