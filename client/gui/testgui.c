//#include "gui.c"
#include "gui.h"
#include "transcriptlist.h"

int main(int argc, char *argv[])
{
    initialize_gui();
    draw_main_interface();
    cleanup_gui();
    return 0;
}

