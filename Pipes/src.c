#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define QNUM	5
#define BUFSIZE 128

#define COL_RESET	   "\x1b[0m"
#define COL_GREEN	   "\x1b[32m"
#define COL_YELLOW	   "\x1b[33m"
#define COL_BLUE	   "\x1b[34m"
#define COL_CYAN	   "\x1b[36m"
#define BOLD_COL_RED   "\x1b[1;31m"
#define BOLD_COL_GREEN "\x1b[1;32m"

const char *questions[QNUM] = {
	"The sun's mass is 1000kg", "The Earth is flat", "This is question #3",
	"Is smog a portmanteau of smoke and fog?",
	"Four Quran Translation courses in a CS degree are pointless"};
const char answers[QNUM] = {'F', 'F', 'T', 'T', 'T'};

int main() {
	srand(time(NULL));

	int pToC[2], cToP[2], ccount = 0, rng = 0, n, ret = 0;
	char buf[BUFSIZE], ans;
	pid_t pid;

	if ((pipe(pToC) == -1) || ((pipe(cToP)) == -1)) {
		perror("pipe()");
		exit(1);
	}

	pid = fork();
	switch (pid) {
	case -1: /* error */
		perror("fork()");
		exit(2);
		break;

	case 0: /* child */
		close(cToP[0]);
		close(pToC[1]);

		for (int i = 0; i < QNUM; i++) {
			if ((n = read(pToC[0], buf, sizeof(buf))) == -1) {
				perror("read()");
				ret = 1;
				goto childCleanup;
			}
			buf[n] = '\0';
			rng	   = rand() % 100 + 1;
			usleep(rng * 10000); /* thinking over question */
			ans = (rng > 50) ? 'T' : 'F';
			printf(COL_BLUE "Ans: %c\n\n" COL_RESET, ans);
			write(cToP[1], &ans, sizeof(ans));
		}

		if ((n = read(pToC[0], &ccount, sizeof(ccount))) == -1) {
			perror("read()");
			ret = 2;
			goto childCleanup;
		}

		if (ccount > 2)
			printf(BOLD_COL_GREEN "Passed!\n" COL_RESET);
		else
			printf(BOLD_COL_RED "Failed!\n" COL_RESET);

	childCleanup:
		close(pToC[0]);
		close(cToP[1]);
		return ret;

	default: /* parent */
		close(cToP[1]);
		close(pToC[0]);

		for (int i = 0; i < QNUM; i++) {
			printf(COL_CYAN "Q%d: %s (%c)\n" COL_RESET, i + 1, questions[i],
				   answers[i]);
			write(pToC[1], questions[i], strlen(questions[i]));
			if ((n = read(cToP[0], &ans, sizeof(ans)) == -1)) {
				perror("read()");
				ret = 1;
				goto parentCleanup;
			}
			ccount += (ans == answers[i]);
		}

		write(pToC[1], (void *)&ccount, sizeof(ccount));

	parentCleanup:
		close(cToP[0]);
		close(pToC[1]);
		return ret;
	}

	return 0;
}
