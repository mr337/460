#include <ncurses.h>
#ifndef GUI_H
#define GUI_H

typedef struct
{
    WINDOW *window;
    int w, h, x, y;
} CHAT_WINDOW;

#define CTRL_6 0x1E
#define CTRL_RB 0x1D
#define CTRL_G 0x07
#define CTRL_H 0x08
#define CTRL_L 0x0C
#define CTRL_N 0X0E
#define CTRL_P 0X10
#define CTRL_Q 0X11
#define CTRL_U 0X15
#define CTRL_W 0x17
#define CTRL_Y 0x19
#define BACKSPACE 128 
#endif
