CC = gcc
CFLAGS = -Wall -Wextra -pedantic
RELEASE_FLAGS = -march=native -O3
DEBUG_FLAGS = -g -O0
LDFLAGS = -lreadline
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)
TARGET = main
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

run:
	./$(TARGET)

all: CFLAGS += $(RELEASE_FLAGS)
all: $(TARGET)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

install: $(TARGET)
	install -d $(BINDIR)
	install -m 755 $(TARGET) $(BINDIR)

uninstall:
	rm -f $(BINDIR)/$(TARGET)

.PHONY: all clean install uninstall run
