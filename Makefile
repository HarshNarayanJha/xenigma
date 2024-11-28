CC = gcc
CFLAGS = -Wall -Wextra -g
CPPFLAGS =

all: bin/xenigma

bin/xenigma: src/main.c | bin
	$(CC) $(CPPFLAGS) $(CFLAGS) src/main.c -o bin/xenigma

bin:
	mkdir -p bin

run: bin/xenigma
	./bin/xenigma

clean:
	rm -f bin/xenigma
