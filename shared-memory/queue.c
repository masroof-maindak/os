#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

#include "queue.h"
#include "utils.h"

struct queue *create_queue(size_t elemSize) {
	struct queue *q = shared_malloc(sizeof(struct queue));
	if (q == MAP_FAILED) {
		perror("mmap() in create_queue() - q");
		return NULL;
	}

	q->header = shared_malloc(sizeof(struct node));
	if (q->header == MAP_FAILED) {
		perror("mmap() in create_queue() - header");
		shared_free(q, sizeof(struct queue));
		return NULL;
	}

	q->sentinel = shared_malloc(sizeof(struct node));
	if (q->sentinel == MAP_FAILED) {
		perror("mmap() in create_queue() - sentinel");
		shared_free(q->header, sizeof(struct node));
		shared_free(q, sizeof(struct queue));
		return NULL;
	}

	q->header->data = q->sentinel->data = NULL;
	q->header->prev = q->sentinel->next = NULL;

	q->header->next	  = q->sentinel;
	q->sentinel->prev = q->header;
	q->size			  = 0;
	q->elemSize		  = elemSize;

	return q;
}

void delete_queue(struct queue *q) {
	struct node *curr = q->header->next;

	while (curr != q->sentinel) {
		struct node *next = curr->next;
		shared_free(curr->data, q->elemSize);
		shared_free(curr, sizeof(struct node));
		curr = next;
	}

	shared_free(q->header, sizeof(struct node));
	shared_free(q->sentinel, sizeof(struct node));

	q->header	= NULL;
	q->sentinel = NULL;
	q->size		= -1;
	q->elemSize = -1;

	shared_free(q, sizeof(struct queue));
	q = NULL;
}

/**
 * @brief adds a ndoe at the end of the queue
 * @param data must point to a chunk of memory that is the same size as the
 * argument used to create a queue object
 */
void enqueue(struct queue *q, const void *data) {
	struct node *new = shared_malloc(sizeof(struct node));
	if (new == MAP_FAILED) {
		perror("mmap() #1 in enqueue()");
		return;
	}

	new->data = shared_malloc(q->elemSize);
	if (new->data == MAP_FAILED) {
		perror("mmap() #2 in enqueue()");
		return;
	}

	struct node *secLast = q->sentinel->prev;
	secLast->next		 = new;
	q->sentinel->prev	 = new;
	new->prev			 = secLast;
	new->next			 = q->sentinel;

	memcpy(new->data, data, q->elemSize);
	q->size++;
}

/**
 * @brief removes the earliest node added
 */
void dequeue(struct queue *q) {
	if (q->size == 0)
		return;

	struct node *n = q->header->next;
	/* FIXME - invalid read/access not within mapped region? */
	q->header->next = n->next;
	n->next->prev	= q->header;

	q->size--;

	shared_free(n->data, q->elemSize);
	n->data = NULL;

	shared_free(n, sizeof(struct node));
	n = NULL;
}

/**
 * @brief returns a copy of the value at the head of the queue
 */
void *top(struct queue *q, void *dst) {
	if (q->size == 0)
		return NULL;
	/* FIXME - invalid read/access not within mapped region? */
	memcpy(dst, q->header->next->data, q->elemSize);
	return dst;
}
