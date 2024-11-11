#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHM_SIZE	2048
#define CHILD_COUNT 6

void *shared_malloc(size_t size) {
	int prot  = PROT_READ | PROT_WRITE;
	int flags = MAP_SHARED | MAP_ANONYMOUS;
	return mmap(NULL, size, prot, flags, -1, 0);
}

void producer_child(void *shmem) {
	for (;;) {
	}

	return;
}

void consumer_child(void *shmem) {
	for (;;) {
	}

	return;
}

int main() {
	void *s;
	pid_t pids[CHILD_COUNT];
	int status;

	s = shared_malloc(SHM_SIZE);
	if (s == MAP_FAILED) {
		perror("mmap()");
		exit(1);
	}

	/* Spawn children */
	for (int i = 0; i < CHILD_COUNT; i++) {
		pids[i] = fork();
		if (pids[i] < 0) {
			perror("fork()");
			exit(2);
		} else if (pids[i] == 0) {
			if (i % 2 == 0)
				producer_child(s);
			else
				consumer_child(s);
			exit(0);
		}
	}

	/* Wait for children to exit */
	for (int i = CHILD_COUNT; i > 0; i--)
		if (wait(&status) == -1)
			perror("wait()");

	return 0;
}
