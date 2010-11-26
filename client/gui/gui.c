#include "gui.h"
#include <ncurses.h>
#include <string.h>
#include <stdarg.h>

CHAT_WINDOW t_win;
CHAT_WINDOW p_win;
CHAT_WINDOW s_win;
CHAT_WINDOW e_win;
CHAT_WINDOW u_wins[10];


void write_to_window(char *message, int window_width, WINDOW *win)
{
    int blank_space = window_width - strlen(message);
    int i;
    wprintw(win, message);
    for (i = 0 ; i < blank_space - 1; i++ ) {
        wprintw(win, " ");
    }
    scrollok(win, 0); //Turn off scrolling to pad out the line
    wprintw(win, " ");
    scrollok(win, 1);
}

void write_line(char *message, int window_width, WINDOW *win)
{
    wprintw(win, " ");
    write_to_window(message, window_width, win);
}

void write_to_windowf(char *message, int window_width, WINDOW *win, ...)
{
    char final_message[50];
    int blank_space;
    va_list list;
    va_start( list, win );
    vsprintf(final_message, message, list);
    va_end( list );
    write_to_window(final_message, window_width, win);
}

void initialize_window(WINDOW *win, int window_height, int window_width)
{
    int i;
    scrollok(win, 1);
    for ( i = 0; i <  window_height- 1; i++ ) {
        write_to_window("", window_width, win);
    }
    write_to_window("", window_width, win);
}

void initialize_colors()
{
    init_pair(1, COLOR_GREEN, COLOR_RED);
    init_pair(2, COLOR_GREEN, COLOR_BLUE);
    init_pair(3, COLOR_BLUE, COLOR_CYAN);
}

void initialize_windows()
{
    initialize_colors();

    t_win.w = 40;
    t_win.h = 23;
    t_win.x = 0;
    t_win.y = 0;
    t_win.window = newwin(t_win.h, t_win.w, t_win.y, t_win.x);

    p_win.w = 40;
    p_win.h = 3;
    p_win.x = 40;
    p_win.y = 0;
    p_win.window = newwin(p_win.h, p_win.w, p_win.y, p_win.x);

    s_win.w = 40;
    s_win.h = 3;
    s_win.x = 40;
    s_win.y = 20;
    s_win.window = newwin(s_win.h, s_win.w, s_win.y, s_win.x);

    e_win.w = 80;
    e_win.h = 1;
    e_win.x = 0;
    e_win.y = 23;
    e_win.window = newwin(e_win.h, e_win.w, e_win.y, e_win.x);

    wcolor_set(t_win.window, 1, NULL);
    wcolor_set(p_win.window, 2, NULL);
    wcolor_set(s_win.window, 3, NULL);
    wcolor_set(e_win.window, 2, NULL);

    initialize_window(t_win.window, t_win.h, t_win.w);
    initialize_window(p_win.window, p_win.h, p_win.w);
    initialize_window(s_win.window, s_win.h, s_win.w);
    initialize_window(e_win.window, e_win.h, e_win.w);

    write_line("This is a test.", t_win.w, t_win.window);

    wrefresh(t_win.window);
    wrefresh(p_win.window);
    wrefresh(s_win.window);
    wrefresh(e_win.window);
}


void draw_main_interface()
{
/*    transcript_win = newwin(23, 40, 0, 0);
    program_info_win = newwin(3, 40, 0, 40);
    status_win = newwin(3, 40, 20, 40);
    editing_win = newwin(1, 80, 23, 0); 

    
    wcolor_set(transcript_win, 1, NULL);
    wcolor_set(program_info_win, 2, NULL);
    wcolor_set(status_win, 3, NULL);
    wcolor_set(editing_win, 2, NULL);

    initialize_window(transcript_win, 23, 40);
    initialize_window(program_info_win, 3, 40);
    initialize_window(status_win, 3, 40);
    initialize_window(editing_win, 1, 80); 


    write_line("This is a test.", 40, transcript_win);
    write_line("Also a test.", 40, transcript_win);

    wrefresh(transcript_win);
    wrefresh(program_info_win);
    wrefresh(status_win);*/
    initialize_windows();
    getch();
}

void initialize_gui()
{
    initscr();
    cbreak();
    if (has_colors() == FALSE)
    {
        endwin();
        printf("Your terminal doesn't have color support");
    }
    else
    {
        start_color();
    }
    refresh();
}

void cleanup_gui()
{
    endwin();
}


