CC = gcc
CFLAGS = -Wall -g

BIN = client 
OBJS = client.c ../networking/networking.c ../gui/gui.c ../gui/transcriptlist.c
LIBS = -lncurses

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS) $(LIBS)

clean:
	rm $(BIN)
