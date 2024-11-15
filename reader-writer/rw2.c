#include "common.h"

const char *colours[] = {YELLOW, RED, BLUE, GREEN, NULL};
volatile int shared	  = 0;

pthread_mutex_t wmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t rmutex = PTHREAD_MUTEX_INITIALIZER;
sem_t resourse, readerAccess;
int readerCount = 0, writerCount = 0;

int main() {
	int ret;
	pthread_t w[THREAD_COUNT], r[THREAD_COUNT];

	srand(time(NULL));

	if (sem_init(&resourse, 0, 1) != 0) {
		perror("sem_init()");
		return 1;
	}

	if (sem_init(&readerAccess, 0, 1) != 0) {
		perror("sem_init()");
		return 1;
	}

	for (int i = 0; i < THREAD_COUNT; i++) {
		ret = pthread_create(&w[i], NULL, fn_write, (void *)colours[i]);
		if (ret != 0) {
			perror("pthread_create()");
			return 3;
		}

		ret = pthread_create(&r[i], NULL, fn_read, (void *)colours[i]);
		if (ret != 0) {
			perror("pthread_create()");
			return 4;
		}
	}

	for (int i = 0; i < THREAD_COUNT; i++) {
		ret = pthread_join(w[i], NULL);
		if (ret != 0) {
			perror("pthread_join()");
			return 5;
		}

		ret = pthread_join(r[i], NULL);
		if (ret != 0) {
			perror("pthread_join()");
			return 6;
		}
	}

	sem_destroy(&readerAccess);
	sem_destroy(&resourse);
	return 0;
}

void *fn_write(void *arg) {
	char *clr = arg;
	int stime;

	for (;;) {
		stime = (rand() % 1200) + 500;

		/* ENTRY */
		//-
		pthread_mutex_lock(&wmutex);
		/* If the first writer gains access, lock out the readers */
		if (++writerCount == 1)
			sem_wait(&readerAccess);
		pthread_mutex_unlock(&wmutex);
		//-

		/* CRITICAL SECTION */
		//-
		sem_wait(&resourse);
		if (shared == 123) {
			printf("%sCounter reset!%s\n", clr, RESET);
			shared = 0;
		} else {
			printf("%sCounter incremented! - %d%s\n", clr, shared, RESET);
			shared++;
			usleep(stime * 100);
		}
		sem_post(&resourse);
		//-

		/* EXIT */
		//-
		pthread_mutex_lock(&wmutex);
		if (--writerCount == 0)
			/* If the last writer releases access, let the readers in */
			sem_post(&readerAccess);
		pthread_mutex_unlock(&wmutex);
		//-
	}

	pthread_exit(NULL);
}

void *fn_read(void *arg) {
	char *clr = arg;
	int stime;

	for (;;) {
		stime = (rand() % 1200) + 500;

		/* ENTRY */
		//-
		sem_wait(&readerAccess);
		//---
		pthread_mutex_lock(&rmutex);
		if (++readerCount == 1)
			/* If first reader's entered, stop writers from entering */
			sem_wait(&resourse);
		pthread_mutex_unlock(&rmutex);
		//---
		sem_post(&readerAccess);
		//-

		/* CRITICAL SECTION */
		printf("%s%d%s\n", clr, shared, RESET);
		usleep(stime * 100);

		/* EXIT */
		//-
		pthread_mutex_lock(&rmutex);
		if (--readerCount == 0)
			/* If the last reader's left, tell writers they can enter */
			sem_post(&resourse);
		pthread_mutex_unlock(&rmutex);
		//-
	}

	pthread_exit(NULL);
}
