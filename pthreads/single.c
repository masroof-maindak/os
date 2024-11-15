#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIMIT 10000000

uint8_t *sieve;

uint8_t is_prime(int n) {
	if (n <= 1)
		return 0;

	for (int i = 2; (long long)i * i <= n; i++)
		if (n % i == 0)
			return 0;

	return 1;
}

int main() {
	sieve = malloc(sizeof(int8_t) * LIMIT);

	if (sieve == NULL) {
		perror("malloc()");
		return 1;
	}

	memset(sieve, 1, sizeof(int8_t) * LIMIT);
	sieve[0] = sieve[1] = 0;

	for (int i = 2; i <= LIMIT; i++)
		if (!is_prime(i))
			sieve[i] = 0;

	/* for (int i = 0; i < LIMIT; i++) */
	/* 	if (sieve[i] != 0) */
	/* 		printf("%d ", i); */

	return 0;
}
