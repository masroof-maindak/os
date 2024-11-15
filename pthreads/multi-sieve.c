#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIMIT		 100
#define THREAD_COUNT 4

uint8_t *sieve;

/* https://stackoverflow.com/a/75266965 */
void *sieve_primes(void *ptr) {
	int tid	  = (int)(intptr_t)ptr;				  // 00, 01, 02, 03
	int start = LIMIT / THREAD_COUNT * tid;		  // 00, 25, 50, 75
	int stop  = LIMIT / THREAD_COUNT * (tid + 1); // 25, 50, 75, 100

	/* set all multiples of odd numbers as composite */
	for (int j = 3; j * j < LIMIT; j += 2) {

		/* start at first multiple of j inside the slice */
		/* integer division 'exploit' */
		int i = ((start + j - 1) / j) * j;

		/* all multiples below j * j have been cleared already */
		if (i < j * j)
			i = j * j;

		/* If `i` is even, add j to it to make it odd */
		if ((i & 1) == 0)
			i += j;

		/*
		 * i is guaranteed to be odd
		 * j is guaranteed to be odd
		 * 2j is even
		 * odd + even = odd
		 *
		 * Therefore, the value at index `i` can not be a prime
		 * Since it is an odd multiple of this j
		 */
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

	int count = 1; // 2

	for (int i = 1; i < LIMIT; i += 2)
		if (sieve[i] == 1)
			count++;

	printf("Count: %d\n", count);
	free(sieve);
	return 0;
}
