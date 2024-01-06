#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../includes/db.h"
#include "../../includes/models.h"
#include "../../includes/backController.h"

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

void freeCredentialsData(Credentials *creds) {
    free(creds->name);
    free(creds->loginName);
    free(creds->password);
    free(creds);
}

void printCreds(Credentials *creds, unsigned int size) {
    if (creds != NULL) {
        for (unsigned int i = 0; i < size; i++) {
            printf("id : %d\tuserId : %d\tname : %s\tlogin : %s\tpassword : %s\n", creds[i].id, creds[i].userId, creds[i].name, creds[i].loginName, creds[i].password);
        }
    } else {
        fprintf(stderr, "creds -> %p\n", creds);
    }
}

int addNewCredsController(MYSQL *dbCon, char *name, char *loginName, char *password) {
    int userId = 1;
    int loginNameSize = strlen(name);
    int loginSize = strlen(loginName);
    int passwordSize = strlen(password);

    if (loginNameSize == 0 || loginNameSize > LOGIN_NAME_MAX_SIZE)
        return EXIT_FAILURE;

    if (loginSize == 0 || loginSize > LOGIN_MAX_SIZE)
        return EXIT_FAILURE;    

    if (passwordSize == 0 || passwordSize > PASSWORD_MAX_SIZE)
        return EXIT_FAILURE;


    Credentials newCreds = { 0, userId, name, loginName, password };
    createNewCreds(dbCon, &newCreds);

    return EXIT_SUCCESS;
}