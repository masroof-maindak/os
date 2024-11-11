#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

struct node {
	char *padding;
	void *data;
	struct node *next;
	struct node *prev;
};

struct queue {
	size_t size;
	size_t elemSize;
	struct node *header;
	struct node *sentinel;
};

struct queue *create_queue(size_t elemSize);
void delete_queue(struct queue *q);
void enqueue(struct queue *q, const void *data);
void dequeue(struct queue *q);
void *top(struct queue *q, void *dst);

#endif // QUEUE_H
