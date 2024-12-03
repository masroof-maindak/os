#ifndef ARENA
#define ARENA

#include <stdbool.h>
#include <stdlib.h>

#define HEAP_SIZE			 1024
#define DEFAULT_FREE_SEG_CAP 8

typedef struct {
	size_t size;
	unsigned char *start;
} info;

typedef struct {
	unsigned char arr[HEAP_SIZE];
	info *freeSegments;
	unsigned int freeSegSize;
	unsigned int freeSegCap;
} arena;

arena *arena_init();
void *arena_alloc(arena *a, size_t n);
bool arena_delete(arena *a, void *mem);
void arena_cleanup(arena *a);

#endif // ARENA
