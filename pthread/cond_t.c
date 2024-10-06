#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define COUNT_DONE	 500
#define COUNT_HALT1	 199
#define COUNT_HALT2	 299
#define RED			 "\033[31m"
#define GREEN		 "\033[32m"
#define BLUE		 "\033[34m"
#define YELLOW		 "\033[1;33m"
#define RESET		 "\033[0m"
#define THREAD_COUNT 4

const char *colours[] = {RED, GREEN, BLUE, YELLOW, NULL};
pthread_mutex_t mut	  = PTHREAD_MUTEX_INITIALIZER;

void *countFn(void *arg);
int count = 0;

int main() {
	pthread_t tid[THREAD_COUNT];

	for (int i = 0; i < THREAD_COUNT; i++)
		pthread_create(&tid[i], NULL, &countFn, (void *)colours[i]);

	for (int i = 0; i < THREAD_COUNT; i++)
		pthread_join(tid[i], NULL);

	printf(RESET "\n");
	exit(0);
}

void *countFn(void *arg) {
	const char *col = arg;

	pthread_mutex_lock(&mut);
	while (count < COUNT_DONE) {
		printf("%s%d " RESET, col, count++);
	}
	pthread_mutex_unlock(&mut);

	return NULL;
}
