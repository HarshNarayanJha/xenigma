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

clean: clean_gen
	rm -f bin/xenigma

clean_gen:
	rm -f test.txt.xor
	rm -f xor.key
	rm -f file.out
	rm -f out.txt
