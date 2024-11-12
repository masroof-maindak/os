#include <limits.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "queue.h"
#include "utils.h"

#define SLEEP_TIME 0.65 * 100000

const char *clrs[] = {RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, NULL};
struct queue *q;
struct semaphores {
	sem_t mutex;
	sem_t filled;
	sem_t empty;
} *shSems;

int init_semaphores();
int cleanup_semaphores();

void consumer_child(const char *col);
void producer_child(const char *col);

int main() {
	pid_t pids[CHILD_COUNT];

	q = create_queue(CHILD_COUNT / 2, sizeof(short));
	if (q == NULL)
		return -1;

	if (init_semaphores() != 0)
		return -2;

	/* Spawn children */
	for (int i = 0; i < CHILD_COUNT; i++) {
		if ((pids[i] = fork()) < 0) {
			perror("fork()");
			return -3;
		} else if (pids[i] == 0) {
			i % 2 == 0 ? producer_child(clrs[i]) : consumer_child(clrs[i]);
			exit(0);
		}
	}

	/* Wait for children to exit */
	for (int i = CHILD_COUNT; i > 0; i--)
		if (wait(NULL) == -1)
			perror("wait()");

	cleanup_semaphores();
	delete_queue(q);
	return 0;
}

void producer_child(const char *col) {
	short *n = malloc(sizeof(short));
	if (n == NULL) {
		perror("malloc() in producer_child()");
		return;
	}

	srand(time(NULL) + getpid());

	printf("%sProducer: started - %d\n" RESET, col, getpid());
	for (;;) {
		*n = rand() % SHRT_MAX;
		usleep(SLEEP_TIME);

		sem_wait(&shSems->empty);
		sem_wait(&shSems->mutex);

		enqueue(q, n);
		printf("%sProducer: pushed %d to queue\n" RESET, col, *n);

		sem_post(&shSems->mutex);
		sem_post(&shSems->filled);
	}

	free(n);
}

void consumer_child(const char *col) {
	short n;

	printf("%sConsumer: started - %d\n" RESET, col, getpid());
	for (;;) {
		sem_wait(&shSems->filled);
		sem_wait(&shSems->mutex);

		n = *(short *)top(q);
		printf("%sConsumer: popped %d from queue\n" RESET, col, n);
		dequeue(q);

		sem_post(&shSems->mutex);
		sem_post(&shSems->empty);

		usleep(SLEEP_TIME);
	}
}

int init_semaphores() {
	shSems = shared_malloc(sizeof(struct semaphores));
	if (shSems == MAP_FAILED) {
		perror("mmap for semaphores failed");
		return -1;
	}

	if (sem_init(&shSems->filled, 1, 0) == -1) {
		perror("sem_init for filled failed");
		return -2;
	}

	if (sem_init(&shSems->empty, 1, CHILD_COUNT / 2) == -1) {
		perror("sem_init for empty failed");
		sem_destroy(&shSems->filled);
		return -3;
	}

	if (sem_init(&shSems->mutex, 1, 1) == -1) {
		perror("sem_init for mutex failed");
		sem_destroy(&shSems->filled);
		sem_destroy(&shSems->empty);
		return -4;
	}

	return 0;
}

int cleanup_semaphores() {
	/* TODO(?): error handling */
	sem_destroy(&shSems->filled);
	sem_destroy(&shSems->empty);
	sem_destroy(&shSems->mutex);
	return shared_free(shSems, sizeof(struct semaphores));
}
