#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_COUNT 3
#define RED			 "\033[31m"
#define GREEN		 "\033[32m"
#define BLUE		 "\033[34m"
#define YELLOW		 "\033[1;33m"
#define RESET		 "\033[0m"

void *fn_write(void *);
void *fn_read(void *);

#endif // COMMON_H
