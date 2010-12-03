CC = gcc
CFLAGS = -Wall -g

BIN = sc
OBJS = singleclient.c networking.c
LIBS = -lncurses

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS) $(LIBS)

clean:
	rm $(BIN)
