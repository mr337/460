#include <ncurses.h>
#include <string.h>
#include <stdarg.h>

WINDOW *transcript_win;
WINDOW *program_info_win;
WINDOW *status_win;
WINDOW *editing_win;
WINDOW *user_wins[10];


void write_to_window(char *message, int window_width, WINDOW *win)
{
    int blank_space = window_width - strlen(message);
    int i;
    scrollok(win, 1);
    wprintw(win, message);
    for (i = 0 ; i < blank_space - 1; i++ ) {
        wprintw(win, " ");
    }
    scrollok(win, 0);
    wprintw(win, " ");
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
    scrollok(win, 0);
    write_to_window("", window_width, win);
}

void draw_main_interface()
{
    transcript_win = newwin(23, 40, 0, 0);
    program_info_win = newwin(3, 40, 0, 40);
    status_win = newwin(3, 40, 20, 40);
    editing_win = newwin(1, 80, 23, 0); 

    init_pair(1, COLOR_GREEN, COLOR_RED);
    init_pair(2, COLOR_RED, COLOR_GREEN);
    init_pair(3, COLOR_BLUE, COLOR_GREEN);
    
    wcolor_set(transcript_win, 1, NULL);
    wcolor_set(program_info_win, 2, NULL);
    wcolor_set(status_win, 3, NULL);
    wcolor_set(editing_win, 2, NULL);

    initialize_window(transcript_win, 23, 40);
    initialize_window(program_info_win, 3, 40);
    initialize_window(status_win, 3, 40);
    initialize_window(editing_win, 1, 80); 


    write_line("This is a test.", 41, transcript_win);
    write_line("Also a test.", 41, transcript_win);

    wrefresh(transcript_win);
    wrefresh(program_info_win);
    wrefresh(status_win);
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


