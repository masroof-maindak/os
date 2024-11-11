#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

#include "queue.h"
#include "utils.h"

struct queue create_queue(size_t numElems, size_t elemSize) {
	printf("creating q w/ %ld elems of size %ld\n", numElems, elemSize);
	struct queue q = {0, numElems, elemSize, NULL};
	q.arr		   = shared_malloc(numElems * elemSize);
	return q;
}

void delete_queue(struct queue q) {
	shared_free(q.arr, q.capacity * q.elemSize);
}

bool enqueue(struct queue q, const void *data) {
	if (q.size >= q.capacity)
		return false;

	memcpy((char *)q.arr + (q.size * q.elemSize), data, q.elemSize);
	q.size++;
	return true;
}

void dequeue(struct queue q) {
	if (q.size > 0) {
		memmove(q.arr, (char *)q.arr + q.elemSize, (q.size - 1) * q.elemSize);
		q.size--;
	}
}

void *top(struct queue q) {
	if (q.size == 0)
		return NULL;
	printf("%p\n", (q.arr + q.size));
	printf("%p\n", (short *)(q.arr + q.size));
	printf("%d\n", *(short *)(q.arr + q.size));
	return (char *)q.arr + (q.size * q.elemSize);
}
