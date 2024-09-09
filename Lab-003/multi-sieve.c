#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

#define LIMIT		1000
#define MAX_THREADS 4

unsigned char sieve[LIMIT];

/* https://stackoverflow.com/a/75266965 */
void *sieveprimes(void *ptr) {
	int n = (int)(intptr_t)ptr;

	/* compute slice boundaries */
	int start = LIMIT / MAX_THREADS * n;
	int stop  = LIMIT / MAX_THREADS * (n + 1);

	/* special case 0, 1 and 2 */
	if (n == 0) {
		sieve[0] = sieve[1] = 0;
		sieve[2]			= 1;
		start				= 3;
	}

	if (n == MAX_THREADS - 1)
		stop = LIMIT;

	/* initialize array slice: only odd numbers may be prime */
	for (int i = start; i < stop; i++)
		sieve[i] = i & 1;

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
	pthread_t tid[MAX_THREADS] = {0};
	for (int i = 0; i < MAX_THREADS; i++) {
		int ret =
			pthread_create(&tid[i], NULL, sieveprimes, (void *)(intptr_t)i);

		if (ret != 0) {
			perror("pthread_create()");
			return 1;
		}
	}

	for (int i = 0; i < MAX_THREADS; i++) {
		int ret = pthread_join(tid[i], NULL);
		if (ret != 0) {
			perror("pthread_join()");
			return 1;
		}
	}

	for (int i = 0; i < LIMIT; i++)
		if (sieve[i] == 1)
			printf("%d ", i);

	return 0;
}
