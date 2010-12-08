//#include "gui.c"
#include "gui.h"
#include "transcriptlist.h"

int main(int argc, char *argv[])
{    
    int q = 0;
    initialize_gui();
    initialize_windows();
    write_to_user_window(0, "This is a chat update and stuff\fThis is status update");
    write_to_user_window(1, "This is another update\fWith some stuff");
    while ( q== 0 )
    {       
        char input = getch();
        switch ( handle_input(input) )
        {
           case CHAT_QUIT:
              q = 1;
              break;
           case CHAT_LURK:
              show_lurk_window();
              break;
           case CHAT_BROADCAST:
              write_to_transcript(message_buffer, 0);
        }
    }
    cleanup_gui();
    return 0;
}

