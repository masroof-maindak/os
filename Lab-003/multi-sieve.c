#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIMIT		 1000
#define THREAD_COUNT 4

uint8_t *sieve;

/* https://stackoverflow.com/a/75266965 */
void *sieve_primes(void *ptr) {
	int tid = (int)(intptr_t)ptr;

	int start = LIMIT / THREAD_COUNT * tid;
	int stop  = LIMIT / THREAD_COUNT * (tid + 1);

	if (tid == THREAD_COUNT - 1)
		stop = LIMIT;

	/* set all multiples of odd numbers as composite */
	for (int j = 3; j * j < LIMIT; j += 2) {

		/* start at first multiple of j inside the slice */
		int i = (start + j - 1) / j * j;

		/* all multiples below j * j have been cleared already */
		if (i < j * j)
			i = j * j;

		/* only handle odd multiples */
		if ((i & 1) == 0)
			i += j;

		for (; i < stop; i += j + j)
			sieve[i] = 0;
	}

	pthread_exit(NULL);
}

int main() {
	sieve = malloc(sizeof(int8_t) * LIMIT);

	if (sieve == NULL) {
		perror("malloc()");
		return 1;
	}

	memset(sieve, 1, sizeof(int8_t) * LIMIT);
	sieve[0] = sieve[1] = 0;

	pthread_t tid[THREAD_COUNT] = {0};
	for (int i = 0; i < THREAD_COUNT; i++) {
		int ret =
			pthread_create(&tid[i], NULL, sieve_primes, (void *)(intptr_t)i);

		if (ret != 0) {
			perror("pthread_create()");
			return 1;
		}
	}

	for (int i = 0; i < THREAD_COUNT; i++) {
		int ret = pthread_join(tid[i], NULL);
		if (ret != 0) {
			perror("pthread_join()");
			return 1;
		}
	}

	for (int i = 0; i < LIMIT; i++)
		if (sieve[i] == 1)
			printf("%d ", i);

	free(sieve);
	return 0;
}
