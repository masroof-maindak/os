#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <sys/mman.h>

#define CHILD_COUNT 6
#define RESET		"\x1b[0m"
#define RED			"\x1b[31m"
#define GREEN		"\x1b[32m"
#define YELLOW		"\x1b[33m"
#define BLUE		"\x1b[34m"
#define MAGENTA		"\x1b[35m"
#define CYAN		"\x1b[36m"

void *shared_malloc(size_t size);
int shared_free(void *addr, size_t len);

#endif // UTILS_H
