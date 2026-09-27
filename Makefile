# Makefile
# AI-GENERATED CODE -- Claude Sonnet 5 (model id: claude-sonnet-5), Anthropic.
# See PROMPT_LOG.md.

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra

all: ipv4prog

ipv4prog: main.o ipv4_extract.o digit_accum.o
	$(CC) $(CFLAGS) -o ipv4prog main.o ipv4_extract.o digit_accum.o

test: tests.o ipv4_extract.o digit_accum.o
	$(CC) $(CFLAGS) -o ipv4test tests.o ipv4_extract.o digit_accum.o
	./ipv4test

main.o: main.c ipv4_extract.h
	$(CC) $(CFLAGS) -c main.c

tests.o: tests.c ipv4_extract.h
	$(CC) $(CFLAGS) -c tests.c

ipv4_extract.o: ipv4_extract.c ipv4_extract.h
	$(CC) $(CFLAGS) -c ipv4_extract.c

digit_accum.o: digit_accum.c ipv4_extract.h
	$(CC) $(CFLAGS) -c digit_accum.c

clean:
	rm -f *.o ipv4prog ipv4test
