#include "gui.h"
#include "../../server/linkedlist.h"
#include "transcriptlist.h"
#include "../networking/networking.h"
#include <ncurses.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

CHAT_WINDOW t_win;
CHAT_WINDOW p_win;
CHAT_WINDOW s_win;
CHAT_WINDOW e_win;
CHAT_WINDOW u_wins[10];
int message_index = 0;
int gaudy_on = 0;

void write_line(char *message, int window_width, WINDOW *win)
{
    wscrl(win, 1);
    wmove(win, getcury(win), 0);
    write_to_window(message, window_width, win);
    wrefresh(win);
}

void write_to_transcript(char *message, int check_for_gaudy)
{
    int count,i;
    while (scrollDown()) {
        scroll_transcript_down();
    } 
           wscrl(t_win.window, 1);
           wmove(t_win.window, getcury(t_win.window), 0);
    count = updateTranscript(message_buffer);
    for ( i = 0; i < count; i++ ) {
        if ( check_for_gaudy == 0 ) {
           write_line(messages[i], t_win.w, t_win.window);
        } else {
           int j;
           int len = strlen(messages[i]);
           int blank_space = t_win.w - len;
          
           for ( j = 0; j < len; j++ ) {
               char input = messages[i][j];
               if ( input == STX ) {
                   blank_space++;
                   wattron(t_win.window, A_REVERSE);
               } else if (input == ETX) {
                   blank_space++;
                   wattroff(t_win.window, A_REVERSE);
               } else if (input != '\0') {
                   waddch(t_win.window, input);
               } else {
                   break;
               }
           }        

           if ( blank_space <= 0 ) {
               scrollok(t_win.window, 0);
            }

           if ( blank_space > 0 )
           {
               int k;
               for (k = 0 ; k < blank_space - 1 ; k++ ) {
                  wprintw(t_win.window, " ");
               }              
               scrollok(t_win.window, 0); //Turn off scrolling to pad out the line
               wprintw(t_win.window, " ");
           }            
           scrollok(t_win.window, 1);
        }
    }
    wrefresh(t_win.window);
}

void write_to_user_window(int user_id, char * message)
{
    int i,j;
    int lastBreak = 0;
    int lastSpace = 0;
    int len = strlen(message);
    int lineCount = 0;
    int gaudy_found = 0;
    char line[(MESSAGELENGTH / u_wins[user_id].w) + 1][u_wins[user_id].w];
    
    for ( i = 0; i < len; i++ )
    {
        if ( len > 0 ) {
            if ( message[i] == ' ' ) {
                lastSpace = i;
            }
        }

        if ( message[i] == STX ) {
            gaudy_found = 1;
        }

        line[lineCount][i - lastBreak] = message[i];
        if ( i == len - 1 ) {
            line[lineCount][i+1] = '\0';
            lineCount++; 
            lastBreak = i;
            lastSpace = i;
        } else if ( (i - lastBreak) == u_wins[user_id].w - 1 ) {
            if ( lastSpace > lastBreak ) {
                line[lineCount][lastSpace - lastBreak] = '\0';
                lastBreak = lastSpace + 1;
                i = lastSpace;
            } else {
                line[lineCount][u_wins[user_id].w] = '\0';
                lastBreak = i + 1;
                lastSpace = i;
            }
            lineCount++;
        }
    }

    j = 0;
    scrollok(u_wins[user_id].window, 0);

    if ( gaudy_found == 0 ) {
      if ( lineCount > 3 ) {
          i = lineCount - 3;
      } else {
          i = 0;
      }

      for ( ; i < lineCount; i++ ) {     
        wmove(u_wins[user_id].window, j, 0);
        wprintw(u_wins[user_id].window, line[i]);
        j++;
      }
    } else {
      int k;
      for ( i = 0; i < lineCount; i++ ) { 
        if ( i >= lineCount - 3) {
          wmove(u_wins[user_id].window, j, 0);
          j++;
        }
        for ( k = 0; k < u_wins[user_id].w; k++ ) {
          if ( line[i][k] == STX ) {
             wattron(u_wins[user_id].window, A_REVERSE);
          } else if ( line[i][k] == ETX) {
             wattroff(u_wins[user_id].window, A_REVERSE);
          } else if ( line[i][k] == '\0' ) {
              break;
          } else if ( i >= lineCount - 3) {        
             waddch(u_wins[user_id].window, line[i][k]);
          }
          wrefresh(u_wins[user_id].window);
        }        
      }
    }

    wrefresh(u_wins[user_id].window);
}

void write_to_window(char *message, int window_width, WINDOW *win)
{
    int blank_space = window_width - strlen(message);
    int i;
    int s = is_scrollok(win);

    if ( blank_space <= 0 )
        scrollok(win, 0);

    wprintw(win, message);
    if ( blank_space > 0 )
    {
       for (i = 0 ; i < blank_space - 1; i++ ) {
           wprintw(win, " ");
       }              
          scrollok(win, 0); //Turn off scrolling to pad out the line
          wprintw(win, " ");
    }
    scrollok(win, s);
}

void write_to_windowh(char *message, int window_width, WINDOW *win)
{
    int blank_space = window_width - strlen(message);
    int i;
    int s = is_scrollok(win);

    wprintw(win, message);
    
    for (i = 0 ; i < blank_space - 1; i++ ) {
        wprintw(win, " ");
    }              
    scrollok(win, 0); //Turn off scrolling to pad out the line
    wprintw(win, " ");
    scrollok(win, s);
}

void write_to_windowf(char *message, int window_width, WINDOW *win, ...)
{  
    char final_message[50];
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

    //clear message_buffer - old chat message was stored there
    memset(message_buffer,0,MESSAGELENGTH);
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

void write_to_program_window(char *message)
{
   int i;
   int len = strlen(message);
   int line = 0;
   scrollok(p_win.window, 0);
   wmove(p_win.window, 0, 0);
   for ( i = 0; i < len; i++ )
   {
       if ( message[i] == '\n' )
       {
          line++;
          wmove(p_win.window, line, 0);
       }
       else
       {
          waddch(p_win.window, message[i]);
       }
   }           
   wrefresh(p_win.window);
}

void write_to_status_window(char *message)
{
   int i;
   int len = strlen(message);
   int line = 0;
   scrollok(s_win.window, 0);
   wmove(s_win.window, 0, 0);
   for ( i = 0; i < len; i++ )
   {
       if ( message[i] == '\n' )
       {
          line++;
          wmove(s_win.window, line, 0);
       }
       else
       {
          waddch(s_win.window, message[i]);
       }
   }           
   wrefresh(s_win.window);
}

int handle_input(char input)
{
    if (iscntrl(input))
    {
          if ( input == CTRL_Q )
          {
              return CHAT_QUIT;
          } else if ( input == ENTER ) {
              int count, i;
              werase(e_win.window);
        write_line(" ", e_win.w, e_win.window);
        wmove(e_win.window, 0,0);
        scrollok(e_win.window, 0);

        //      write_to_transcript(message_buffer, 1);
         //     message_buffer[0] = '\0';
              message_index = 0;        
              return CHAT_BROADCAST;
          } else if ( input == CTRL_L ) {
              write_to_transcript("Lurk!", 0);
          } else if ( input == CTRL_P ) {
              if ( scrollUp() == 1) {
                wscrl(t_win.window, -1);
                char *line = getTop();
                wmove(t_win.window, 0, 0);
                write_to_window(line, t_win.w, t_win.window);                
                wrefresh(t_win.window);
              }
          } else if ( input == CTRL_N ) {
              if ( scrollDown() == 1) {                  
                  scroll_transcript_down();
              }  
          } else if ( input == BACKSPACE ) {
              if (message_index > 0) {
                  message_index--;
                  message_buffer[message_index] = '\0';
                  int x = getcurx(e_win.window);
                  if ( x > 1 ) {
                    wmove(e_win.window, getcury(e_win.window), x - 1);
                    waddch(e_win.window, ' ');
                    wmove(e_win.window, getcury(e_win.window), x - 1);
                  } else {
                     int newx;
                     wmove(e_win.window, 0, 0);
                     if ( message_index >= e_win.w ) {
                         newx = e_win.w - 1;
                         write_to_windowh(message_buffer + (message_index - e_win.w) +1,
                                 e_win.w, e_win.window);
                     } else {
                         newx = message_index;
                         write_to_windowh(message_buffer,
                                 e_win.w, e_win.window);
                     }
                     wmove(e_win.window, 0, newx);
                     wrefresh(e_win.window);
                  }
              }               
          } else if ( input == CTRL_G ) {        
              if ( gaudy_on == 0 ) { 
                  message_buffer[message_index++] = STX;
                  gaudy_on = 1;
              } else {
                  message_buffer[message_index++] = ETX;
                  gaudy_on = 0;
              }        
          }
        }

        else if (message_index < MESSAGELENGTH)        
        {
            if ( message_index == MESSAGELENGTH - 1 )
            {
                message_buffer[message_index++] = '\0';
            } else {
              message_buffer[message_index++] = input;
              message_buffer[message_index] = '\0';            
              if ( message_index < 80 || getcurx(e_win.window) < e_win.w - 1 ) {
                  waddch(e_win.window, input);
              } else {
                  scrollok(e_win.window, 1);
                  wmove(e_win.window, e_win.y, 0);
                  write_to_windowh(message_buffer + (message_index - e_win.w),
                          e_win.w, e_win.window);
                  scrollok(e_win.window, 0);
              }
            }
        }
    
    //return CHAT_BROADCAST;
    return CHAT_UPDATE;
}

void draw_main_interface()
{
    initialize_windows();
    write_to_program_window("This is\nA Test\nand junk");
    write_to_status_window("This is\nThe Status Window\nYay");
    write_to_user_window(0, "This is a test \x02of writing to the user window and\x03 i hope this works");
    while ( 1 )
    {
        char input = wgetch(e_win.window);    
        if (handle_input(input) == CHAT_QUIT)
            break;
    }
}

void scroll_transcript_down()
{
  wscrl(t_win.window, 1);          
  char *line = getBottom();
  wmove(t_win.window, 22, 0);
  write_to_window(line, t_win.w, t_win.window);
  wrefresh(t_win.window);
}

void initialize_gui()
{
    initscr();
    raw();
    noecho();
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


