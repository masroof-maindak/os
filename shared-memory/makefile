CFLAGS = -Wall -Wextra -pedantic
LDFLAGS = -lpthread
DEBUG_FLAGS = -g3

SRCS = src.c queue.c utils.c

default:
	gcc $(SRCS) $(CFLAGS) $(LDFLAGS)

debug:
	gcc $(SRCS) $(CFLAGS) $(DEBUG_FLAGS) $(LDFLAGS)

clean:
	rm -f *.o a.out

