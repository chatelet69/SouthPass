#include <stdio.h>
#include <stdlib.h>
#include "../includes/pincludes.h"

int main(int argc, char **argv)
{
    int status = EXIT_SUCCESS;

    ApplicationController ApplicationController(argc, argv);
    status = ApplicationController.run();

    return status;
}
