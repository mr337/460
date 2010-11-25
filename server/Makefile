CC = gcc
CFLAGS = -Wall -g
LIBS = -lpthread

BIN = server 
OBJS = server.c linkedlist.c 

all: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(BIN) $(OBJS)

clean:
	rm $(BIN) 

