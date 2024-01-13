#include <stdio.h>
#include <stdlib.h>
#include "../../includes/models.h"
#include "../../includes/utils.h"

void removeReturnLine(char *str) {
    for (int i = 0; str[i] != '\0'; i++) if (str[i] == '\n') str[i] = '\0';
}

int getSizeInString(int num) {
    int count = 0;
    while (num > 0) {
        num /= 10;
        count++;
    }
    return count;
}

void printCred(Credentials *credentials) {
    printf("id : %d\tuser id : %d\tname : %s\tlogin : %s\tpassword : %s\n", credentials->id, credentials->userId, credentials->name, credentials->loginName, credentials->password);
}