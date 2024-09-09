#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIMIT		 1000
#define THREAD_COUNT 4

uint8_t *sieve;

uint8_t is_prime(int n) {
	if (n <= 1)
		return 0;

	for (int i = 2; (long long)i * i <= n; i++)
		if (n % i == 0)
			return 0;

	return 1;
}

void *split_prime_checking(void *ptr) {
	int tid	  = (int)(intptr_t)ptr;
	int start = (LIMIT / THREAD_COUNT) * tid;
	int stop  = (LIMIT / THREAD_COUNT) * (tid + 1);

	for (int i = start; i <= stop; i++)
		if (!is_prime(i))
			sieve[i] = 0;

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

	pthread_t tid[THREAD_COUNT];

	for (int i = 0; i < THREAD_COUNT; i++) {
		int ret = pthread_create(&tid[i], NULL, split_prime_checking,
								 (void *)(intptr_t)i);
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

	return 0;
}
