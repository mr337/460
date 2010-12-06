#include <ncurses.h>
#include "../networking/networking.h"
#ifndef GUI_H
#define GUI_H

typedef struct
{
    WINDOW *window;
    int w, h, x, y;
    chtype color;
} CHAT_WINDOW;

#define CHAT_UPDATE 0
#define CHAT_QUIT 1
#define CHAT_BROADCAST 2
#define CHAT_LURK 5 
#define YELL 6
#define CHAT_GAUDY 7
#define DS_REQUEST 3
#define DS_VOTE 4

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
#define BACKSPACE 127 
#define ENTER 0xA
#define STX 2
#define ETX 3

void write_to_window(char *message, int window_width, WINDOW *win);
void write_line(char *message, int window_width, WINDOW *win);
void write_to_transcript(char *message, int check_for_gaudy);
void write_to_windowf(char *message, int window_width, WINDOW *win, ...);
void initialize_window(WINDOW *win, int window_height, int window_width);
void initialize_colors();
void initialize_windows();
void draw_main_interface();
void initialize_gui();
void cleanup_gui();
void scroll_transcript_down();
int handle_input(char input);
int handle_deepsix_input(char input);
int handle_chat_input(char input);

void write_to_user_window(int user_id, char * message);
void write_to_program_window(char * message);
void write_to_status_window(char * message);
void show_ds_window(char * message);
void show_eject_window(char * message);

char message_buffer[MESSAGELENGTH];
int ds_vote;

#endif
