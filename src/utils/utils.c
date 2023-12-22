#include <stdio.h>
#include <stdlib.h>
#include "../../includes/utils.h"

void removeReturnLine(char *str) {
    for (int i = 0; str[i] != '\0'; i++) if (str[i] == '\n') str[i] = '\0';
}