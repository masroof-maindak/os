#include "utils.h"

void *shared_malloc(size_t size) {
	int prot  = PROT_READ | PROT_WRITE;
	int flags = MAP_SHARED | MAP_ANONYMOUS;
	return mmap(NULL, size, prot, flags, -1, 0);
}

int shared_free(void *addr, size_t len) { return munmap(addr, len); }
