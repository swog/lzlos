#include "kdefs.h"
#include "queue.h"

void queue_init(
	queue_t *queue, 
	allocator_t *alloc, 
	deallocator_t *dealloc
) {
	queue->alloc = alloc;
	queue->dealloc = dealloc;
	queue->head = queue->tail = NULL;
	queue->size = 0;
}

void queue_free(queue_t *queue) {
	queue_entry_t *next;	

	while (queue->head) {
		next = queue->head->next;
		queue->dealloc(queue->head);
		queue->head = next;
	}

	queue->head = queue->tail = NULL;
}

void *queue_pop(queue_t *queue) {
	queue_entry_t *entry = queue->head;

	queue->head = entry->next;

	if (!queue->head) {
		queue->tail = NULL;
	}

	void *data = entry->data;

	queue->dealloc(entry);

	//if (queue->size) {
		queue->size--;
	//}

	return data;
}

void queue_push(queue_t *queue, void *data) {
	queue_entry_t *entry = (queue_entry_t*)queue->alloc(sizeof(queue_entry_t));

	entry->data = data;

	if (!queue->tail) {
		entry->next = entry->prev = NULL;	
		queue->tail = queue->head = entry;
	}
	else {
		entry->prev = queue->tail;
		entry->next = NULL;

		queue->tail->next = entry;
		queue->tail = entry;
	}

	queue->size++;	
}

void *queue_top(queue_t *queue) {
	if (queue->head) {
		return queue->head->data;
	}
	return NULL;
}

unsigned int queue_size(const queue_t *queue) {
	return queue->size;
}
