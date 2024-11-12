#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

#include "queue.h"
#include "utils.h"

struct queue *create_queue(size_t numElems, size_t elemSize) {
	struct queue *q = shared_malloc(sizeof(struct queue));
	if (q == MAP_FAILED)
		return NULL;

	q->capacity = numElems;
	q->elemSize = elemSize;
	q->size		= 0;
	q->arr		= shared_malloc(numElems * elemSize);

	if (q->arr == MAP_FAILED) {
		shared_free(q, sizeof(struct queue));
		return NULL;
	}

	return q;
}

void delete_queue(struct queue *q) {
	shared_free(q->arr, (q->capacity * q->elemSize));
	shared_free(q, sizeof(struct queue));
}

bool enqueue(struct queue *q, const void *data) {
	if (q->size >= q->capacity)
		return false;

	memcpy((char *)q->arr + (q->size * q->elemSize), data, q->elemSize);
	q->size++;
	return true;
}

void dequeue(struct queue *q) {
	if (q->size > 0) {
		memmove(q->arr, (char *)q->arr + q->elemSize,
				(q->size - 1) * q->elemSize);
		q->size--;
	}
}

void *top(struct queue *q) {
	if (q->size == 0)
		return NULL;
	return (void *)q->arr;
}
