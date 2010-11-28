#include "gui.h"
#include "../../server/linkedlist.h"
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
    wrefresh(win);
}

void write_to_transcript(char *message)
{
    write_line(message, t_win.w, t_win.window);
    write_line(" ", e_win.w, e_win.window);
    wmove(e_win.window, 0,0);
    scrollok(e_win.window, 0);
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
    init_pair(4, COLOR_BLACK, COLOR_YELLOW);
    init_pair(5, COLOR_BLACK, COLOR_WHITE);
}

void initialize_windows()
{
    int i, j;
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

    wrefresh(t_win.window);
    wrefresh(p_win.window);
    wrefresh(s_win.window);
    wrefresh(e_win.window);

    for ( i = 0; i < 2; i++ ) {
        for ( j = 0; j < 5; j++ ) {
            int index = j + (5 * i);
            int color = 4 + (index % 2);
            u_wins[index].w = 20;
            u_wins[index].h = 3;
            u_wins[index].x = 40 + (20 * i);
            u_wins[index].y = j * 3 + 4;
            u_wins[index].window = newwin(u_wins[index].h,
                                          u_wins[index].w,
                                          u_wins[index].y,
                                          u_wins[index].x);

            wcolor_set(u_wins[index].window, color, NULL);
            initialize_window(u_wins[index].window,
                              u_wins[index].h,
                              u_wins[index].w);
            wrefresh(u_wins[index].window);
        }
    }
    scrollok(e_win.window, 0);
    wmove(e_win.window, 0, 0);    
}


void draw_main_interface()
{
    char message_buffer[100];
    int message_index = 0;
    initialize_windows();
    while ( 1 )
    {
        char input = wgetch(e_win.window);    
        if ( input == CTRL_Q )
        {
            break;
        } else if ( input == 0xA ) {
            werase(e_win.window);
            write_to_transcript(message_buffer);
            message_buffer[0] = '\0';
            message_index = 0;        
        } else if ( input == CTRL_L ) {
            write_to_transcript("Lurk!");
        } else if ( input == CTRL_P ) {
            wscrl(t_win.window, 1);
            wrefresh(t_win.window);
        } else if ( input == CTRL_N ) {
            wscrl(t_win.window, -1);
            wrefresh(t_win.window);
        } else {
            message_buffer[message_index++] = input;
            message_buffer[message_index] = '\0';
        }
    }
}

void initialize_gui()
{
    initscr();
    raw();
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


