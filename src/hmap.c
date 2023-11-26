//===============================================
// Hash map implementation
// 
// Used in reverse memory map searching for blocks.
//===============================================

#include "kdefs.h"
#include "alloc.h"
#include "hmap.h"

//===============================================
// Hash map free
//===============================================
void hmap_free(hmap_t *map, int free_list) {
	hmap_bucket_t *entry, *next;

	for (int i = 0; i < map->count; i++) {
		entry = map->list[i];

		while (entry) {
			next = entry->next;

			map->dealloc(entry);
			entry = next;
		}

		map->list[i] = NULL;
	}

	if (free_list) {
		map->dealloc(map->list);
	}
}

//===============================================
// Initialize hash map
//===============================================
void hmap_init(
	// Map
	hmap_t*				map, 
	// Map count
	unsigned short 		count,
	// Allocate list boolean
	int 				alloc_list,
	// Allocator
	allocator_t*		alloc, 
	// Deallocator
	deallocator_t*		dealloc, 
	// Hash map
	hmap_hash_t*		hash,
	// Key comparator
	hmap_compare_t*		compare
) {
	map->count = count;
	map->alloc = alloc;
	map->dealloc = dealloc;
	map->hash = hash;
	map->compare = compare;
	
	if (alloc_list) {
		map->list = (hmap_bucket_t**)alloc(count * sizeof(hmap_bucket_t*));
	}

	for (int i = 0; i < map->count; i++) {
		map->list[i] = NULL;
	}
}

//===============================================
// Hash map search
// Return NULL if not found
//=============================================== 
void *hmap_at(hmap_t *map, const void *key) {
	unsigned int index = map->hash(key) % map->count;
	hmap_bucket_t *entry = map->list[index];

	while (entry) {
		if (!map->compare(entry->key, key)) {
			return entry->data;
		}

		entry = entry->next;
	}

	return NULL;
}

#include "vga.h"

//===============================================
// Insert hmap
// Doesn't remove duplicates (CBA)
//===============================================
void hmap_insert(hmap_t *map, const void *key, void *data) {
	unsigned int index = map->hash(key) % map->count;
	hmap_bucket_t *entry = (hmap_bucket_t*)map->alloc(sizeof(hmap_bucket_t));

	entry->key = key;	
	entry->data = data;

	if (map->list[index]) {
		map->list[index]->prev = entry;
	}

	entry->next = map->list[index];
	entry->prev = NULL;
	map->list[index] = entry;
}

//===============================================
// 	Remove hmap entry. O(n) if all entries are
// in the same bucket.
//===============================================
void hmap_remove(hmap_t *map, const void *key) {
	unsigned int index = map->hash(key) % map->count;
	hmap_bucket_t *entry = map->list[index];

	// Iterate each bucket entry
	while (entry) {
		// Compare entry & data
		if (!map->compare(entry->key, key)) {
			// Remove head
			if (entry == map->list[index]) {
				map->list[index] = entry->next;
				// Remove prev
				if (entry->next) {
					entry->next->prev = NULL;
				}
			}
			else {
				// Relink next
				if (entry->next) {
					entry->next->prev = entry->prev;
				}
				// Relink previous
				if (entry->prev) {
					entry->prev->next = entry->next;
				}
			}
			// Deallocate
			map->dealloc(entry);
			break;
		}

		entry = entry->next;
	}
}







