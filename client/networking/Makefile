CC = gcc
CFLAGS = -Wall -g

BIN = client
OBJS = client.c networking.c

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS)

clean:
	rm $(BIN) 
