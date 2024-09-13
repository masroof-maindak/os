#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t count_mutex		= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond				= PTHREAD_COND_INITIALIZER;

void *functionCount1(void *arg);
void *functionCount2();
int count = 0;

#define COUNT_DONE	400
#define COUNT_HALT1 99
#define COUNT_HALT2 177

#define BOLD_RED	"\033[1m\033[31m"
#define BOLD_GREEN	"\033[1m\033[32m"
#define BOLD_YELLOW "\033[1m\033[33m"
#define BOLD_BLUE	"\033[1m\033[34m"
#define RESET		"\033[0m"

int main() {
	pthread_t thread1, thread2, thread3, thread4;

	pthread_create(&thread1, NULL, &functionCount1, BOLD_RED);
	pthread_create(&thread3, NULL, &functionCount1, BOLD_GREEN);
	pthread_create(&thread4, NULL, &functionCount1, BOLD_BLUE);
	pthread_create(&thread2, NULL, &functionCount2, NULL); /* signaller */
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);

	printf(RESET "\n");
	exit(0);
}

void *functionCount1(void *arg) {
	const char *col = arg;

	while (1) {
		pthread_mutex_lock(&condition_mutex);
		while (count >= COUNT_HALT1 && count <= COUNT_HALT2)
			pthread_cond_wait(&cond, &condition_mutex);
		pthread_mutex_unlock(&condition_mutex);

		pthread_mutex_lock(&count_mutex);
		count++;
		printf("%s%d ", col, count);
		pthread_mutex_unlock(&count_mutex);

		if (count >= COUNT_DONE)
			return (NULL);
	}
}

void *functionCount2() {
	while (1) {
		pthread_mutex_lock(&condition_mutex);
		if (count < COUNT_HALT1 || count > COUNT_HALT2)
			pthread_cond_signal(&cond);
		pthread_mutex_unlock(&condition_mutex);

		pthread_mutex_lock(&count_mutex);
		count++;
		printf(BOLD_YELLOW "%d ", count);
		pthread_mutex_unlock(&count_mutex);

		if (count >= COUNT_DONE)
			return (NULL);
	}
}
