CC = gcc
CFLAGS = -Wall -g

BIN = client
OBJS = client.c networking.c

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS)

sc:
	$(CC) $(CFLAGS) -o sc singleclient.c networking.c -lncurses

clean:
	rm $(BIN) 
