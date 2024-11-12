CFLAGS = -Wall -Wextra -pedantic
LDFLAGS = -lpthread
DEBUG_FLAGS = -g3
RELEASE_FLAGS = -march=native -O3

SRCS = src.c queue.c utils.c
TARGET = shm

default: debug

$(TARGET):
	gcc $(SRCS) $(CFLAGS) $(RELEASE_FLAGS) $(LDFLAGS) -o $(TARGET)

debug:
	gcc $(SRCS) $(CFLAGS) $(DEBUG_FLAGS) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f *.o a.out
