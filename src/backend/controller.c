#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/sha.h>
#include <string.h>
#include "../../includes/db.h"
#include "../../includes/models.h"
#include "../../includes/backController.h"
#include "../../includes/fileController.h"

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

int getUserIdByToken(MYSQL *dbCon) {
    TokenInfos *tokenInfos = getTokenFileInfos();

    if (tokenInfos != NULL) {
        int res = getUserByTokenInfos(dbCon, tokenInfos->token, tokenInfos->id);
        printf("aze\n");
        int id = tokenInfos->id;
        free(tokenInfos);
        if (res == 1) return id;
    }
    printf("endGetUserIdByToken\n");

    return 0;
}

char *getActualDate() {
    time_t t = time(NULL);
    struct tm *actualTime = localtime(&t);
    char *actualDateStr = (char *) malloc(sizeof(char) * 20);
    strftime(actualDateStr, 20, "%Y-%m-%d", actualTime);
    return actualDateStr;
}

int generateNewUserToken(MYSQL *dbCon, char *userEmail) {
    int userId = getUserIdBy(dbCon, userEmail, "email");
    printf("USER ID : %d\n", userId);

    if (userId != 0) {
        char *actualDateStr = getActualDate();
        char *baseToken = (char *) malloc(sizeof(char) * 257);
        srand(time(NULL));
        int randomVal = rand() % (2000) + 1000;
        sprintf(baseToken, "%s_%d_%s_%d", userEmail, userId, actualDateStr, randomVal);
        char tokenHash[257];
        
        char *hashString = (char*)malloc(2*SHA256_DIGEST_LENGTH+1);
        strcpy(tokenHash, shaPwd(baseToken, hashString, actualDateStr));
        
        free(hashString);
        free(baseToken);
        free(actualDateStr);

        saveNewUserTokenDb(dbCon, userId, tokenHash);
        saveNewTokenFile(tokenHash, userEmail, userId);

        return EXIT_SUCCESS;
    } else {
        return -1;
    }
}