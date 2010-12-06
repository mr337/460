//#include "gui.c"
#include "gui.h"
#include "transcriptlist.h"

int main(int argc, char *argv[])
{
    initialize_gui();
    initialize_windows();
    while ( 1 )
    {
        char input = getch();
        if ( handle_input(input) == CHAT_QUIT )
            break;
        else
            write_to_user_window(0, message_buffer);
    }
    cleanup_gui();
    return 0;
}

