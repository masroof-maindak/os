#include <stdlib.h>
#include <string.h>

#include "arena.h"

arena *arena_init() {
	arena *a = malloc(sizeof(*a));
	if (a == NULL)
		return NULL;

	memset(a->arr, 0, HEAP_SIZE);
	a->freeSegSize	= 1;
	a->freeSegCap	= DEFAULT_FREE_SEG_CAP;
	a->freeSegments = malloc(sizeof(info) * a->freeSegCap);

	if (a->freeSegments == NULL) {
		a->freeSegSize = 0;
		free(a);
		return NULL;
	}

	a->freeSegments[0] = (info){HEAP_SIZE, a->arr};

	return a;
}

void *arena_alloc(arena *a, size_t n) {
	/* check if we have an opening of enough size */
	unsigned short i = 0;
	for (; i < a->freeSegSize; i++)
		if (a->freeSegments[i].size > n + sizeof(n))
			break;

	if (i == a->freeSegSize)
		return NULL;

	/* write size before allocation */
	size_t *freeAddress = (size_t *)a->freeSegments[i].start;
	freeAddress[0]		= n;

	/* modify info array */
	if (a->freeSegments[i].size == n + sizeof(n)) {
		for (unsigned short j = i; j < a->freeSegSize - 1; j++)
			a->freeSegments[j] = a->freeSegments[j + 1];
		a->freeSegSize--;
	} else {
		a->freeSegments[i].size -= (n + sizeof(n));
		a->freeSegments[i].start += (n + sizeof(n));
	}

	return freeAddress + 1;
}

bool arena_delete(arena *a, void *mem) {
	void *tmp	= NULL;
	size_t size = *(size_t *)mem - 1;
	a->freeSegments[a->freeSegSize++] =
		(info){size + sizeof(size), (unsigned char *)mem};

	if (a->freeSegSize > a->freeSegCap) {
		a->freeSegCap *= 2;
		tmp = realloc(a->freeSegments,
					  sizeof(a->freeSegments[0]) * a->freeSegCap);
		if (tmp == NULL) {
			free(a->freeSegments);
			return false;
		}
		a->freeSegments = tmp;
	}

	return true;
}

void arena_cleanup(arena *a) {
	free(a->freeSegments);
	a->freeSegments = NULL;
	a->freeSegSize	= 0;
	a->freeSegCap	= 0;
	free(a);
}
