#include <stdio.h>
#include <stdlib.h>
#include "../../includes/db.h"
#include "../../includes/controler.h"

char **getPwsdList() {
    printf("e\n");
    return NULL;
}

void freeCredsArray(CredsArray credsArray) {
    for (unsigned int i = 0; i < credsArray.size; i++) {
        free(credsArray.creds[i].name);
        free(credsArray.creds[i].loginName);
        free(credsArray.creds[i].password);
    }
    free(credsArray.creds);
}

void printCreds(Credentials *creds, unsigned int size) {
    if (creds != NULL) {
        for (unsigned int i = 0; i < size; i++) {
            printf("id : %d\tname : %s\tlogin : %s\tpassword : %s\n", creds[i].id, creds[i].name, creds[i].loginName, creds[i].password);
        }
    } else {
        fprintf(stderr, "creds -> %p\n", creds);
    }
}