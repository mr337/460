CC = gcc
CFLAGS = -Wall -g 

BIN = testgui
OBJS = testgui.c gui.c transcriptlist.c
LIBS = -lcurses

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS) $(LIBS)

clean:
	rm $(BIN) 
	 
