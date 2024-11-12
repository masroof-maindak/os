#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

#include "bool.h"

struct queue {
	size_t size;
	size_t capacity;
	size_t elemSize;
	void *arr;
};

struct queue *create_queue(size_t numElems, size_t elemSize);
void delete_queue(struct queue *q);
bool enqueue(struct queue *q, const void *data);
void dequeue(struct queue *q);
void *top(struct queue *q);

#endif // QUEUE_H
