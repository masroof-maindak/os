CFLAGS = -Wall -Wextra -pedantic
DEBUG_FLAGS = -g3
RELEASE_FLAGS = -march=native -O3

SRCS = src.c arena.c
TARGET = arena

default: debug

$(TARGET):
	gcc $(SRCS) $(CFLAGS) $(RELEASE_FLAGS) -o $(TARGET)

debug:
	gcc $(SRCS) $(CFLAGS) $(DEBUG_FLAGS) -o $(TARGET)

clean:
	rm -f arena
