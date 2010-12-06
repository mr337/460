//#include "gui.c"
#include "gui.h"
#include "transcriptlist.h"

int main(int argc, char *argv[])
{
    int q = 0;
    initialize_gui();
    initialize_windows();
    while ( q== 0 )
    {       
        char input = getch();
        switch ( handle_input(input) )
        {
           case CHAT_QUIT:
              q = 1;
              break;
           case CHAT_BROADCAST:
              show_eject_window("You're outta here!");
              break;
           default: 
            write_to_user_window(0, message_buffer);
        }
    }
    cleanup_gui();
    return 0;
}

