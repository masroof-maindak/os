#include <stdio.h>

#include "arena.h"

void test_arena_initialization() {
	printf("\nRunning test: Arena Initialization\n");
	arena *a = arena_init();

	if (a->freeSegments == NULL) {
		printf("Test failed: freeSegments not initialized.\n");
		return;
	}

	if (a->freeSegSize != 1 || a->freeSegments[0].size != HEAP_SIZE) {
		printf("Test failed: Initial free segment not set correctly.\n");
		return;
	}

	arena_cleanup(a);
	printf("Test passed.\n");
}

void test_arena_allocation() {
	printf("\nRunning test: Arena Allocation\n");
	arena *a = arena_init();

	void *block = arena_alloc(a, 64);
	if (block == NULL) {
		printf("Test failed: Allocation returned NULL.\n");
		return;
	}

	size_t size = *((size_t *)block - 1);
	if (size != 64) {
		printf("Test failed: Allocated size mismatch.\n");
		return;
	}

	arena_cleanup(a);
	printf("Test passed.\n");
}

void test_arena_deallocation() {
	printf("\nRunning test: Arena Deallocation\n");
	arena *a = arena_init();

	void *block = arena_alloc(a, 64);
	if (block == NULL) {
		printf("Test failed: Allocation returned NULL.\n");
		return;
	}

	if (!arena_delete(a, block)) {
		printf("Test failed: Deallocation failed.\n");
		return;
	}

	if (a->freeSegSize != 2) {
		printf(
			"Test failed: Free segment size not updated after deallocation.\n");
		return;
	}

	arena_cleanup(a);
	printf("Test passed.\n");
}

int main() {
	test_arena_initialization();
	test_arena_allocation();
	test_arena_deallocation();

	return 0;
}
