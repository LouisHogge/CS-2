SRCS = board.c bejeweled.c hint.c play.c
OBJS = $(SRCS:%.c=%.o)
CC = gcc
PROG = bejeweled
CFLAGS = -Wall -Wextra -Wmissing-prototypes --pedantic -std=c99 -g
LDFLAGS = -lm

.PHONY: all clean run

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS) $(PROG)

run: $(PROG)
	./$(PROG) 8 9 1 10 0 0 1

bejeweled.o: bejeweled.c board.h hint.h play.h
board.o: board.c board.h
hint.o: hint.c hint.h board.h
play.o: play.c play.h board.h

