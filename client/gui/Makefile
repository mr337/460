CC = gcc
CFLAGS = -Wall -g 

BIN = testgui
OBJS = testgui.c gui.c
LIBS = -lcurses

all: $(OBJS)
	$(CC) $(CFALGS) -o $(BIN) $(OBJS) $(LIBS)

clean:
	rm $(BIN) 
	 
