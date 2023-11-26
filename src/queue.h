
#pragma once
#include "alloc.h"

typedef struct queue_entry_s {
	void *data;
	struct queue_entry_s *next, *prev;
} queue_entry_t;

typedef struct queue_s {
	allocator_t *alloc;
	deallocator_t *dealloc;
	unsigned int size;
	queue_entry_t *head, *tail;
} queue_t;

void queue_init(queue_t *queue, allocator_t *alloc, deallocator_t *dealloc);
void queue_free(queue_t *queue);
void *queue_pop(queue_t *queue);
void queue_push(queue_t *queue, void *data);
void *queue_top(queue_t *queue);
unsigned int queue_size(const queue_t *queue);

