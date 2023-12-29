#include <stdio.h>
#include <stdlib.h>
#include "../includes/pincludes.h"

int main(int argc, char **argv)
{
    int status = EXIT_SUCCESS;

    GuiService guiService(argc, argv);
    status = guiService.run();

    return status;
}
