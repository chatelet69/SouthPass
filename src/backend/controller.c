#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/sha.h>
#include <string.h>
#include "../../includes/db.h"
#include "../../includes/models.h"
#include "../../includes/backController.h"
#include "../../includes/webService.h"
#include "../../includes/pincludes.h"
#include "../../includes/fileController.h"
#include "../../includes/cryptoModule.h"

void freeCredsArray(struct CredsArray *credsArray) {
    if (credsArray != NULL) {
        for (unsigned int i = 0; i < credsArray->size; i++) {
            //printf("%s %s %s\n", credsArray->credentials[i].name, credsArray->credentials[i].loginName, credsArray->credentials[i].password);
            free(credsArray->credentials[i].name);
            free(credsArray->credentials[i].loginName);
            free(credsArray->credentials[i].password);
        }
        free(credsArray->credentials);
        credsArray->size = 0;
    }
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
    const int userId = getUserIdByCookieFile();
    int loginNameSize = strlen(name);
    int loginSize = strlen(loginName);
    int passwordSize = strlen(password);

    if (loginNameSize == 0 || loginNameSize > LOGIN_NAME_MAX_SIZE)
        return EXIT_FAILURE;

    if (loginSize == 0 || loginSize > LOGIN_MAX_SIZE)
        return EXIT_FAILURE;    

    if (passwordSize == 0 || passwordSize > PASSWORD_MAX_SIZE)
        return EXIT_FAILURE;

    //unsigned char *finalPass = encryptString(password, "coucou");
    if (userId != 0) {
        Credentials newCreds = { 0, userId, name, loginName, password };
        createNewCreds(dbCon, &newCreds);
    } else {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int getUserIdByToken(MYSQL *dbCon) {
    TokenInfos *tokenInfos = getTokenFileInfos();

    if (tokenInfos != NULL) {
        int res = getUserByTokenInfos(dbCon, tokenInfos->token, tokenInfos->id);
        int id = tokenInfos->id;
        free(tokenInfos);
        if (res == 1) return id;
    }

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
    char emailOption[6];
    strcpy(emailOption, "email");
    int userId = getUserIdBy(dbCon, userEmail, emailOption);
    printf("USER ID : %d\n", userId);

    if (userId != 0) {
        char *actualDateStr = getActualDate();
        char *baseToken = (char *) malloc(sizeof(char) * 65);
        srand(time(NULL));
        int randomVal = rand() % (2000) + 1000;
        sprintf(baseToken, "%s_%d_%s_%d", userEmail, userId, actualDateStr, randomVal);
        char tokenHash[65];
        
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

int importPasswordsController(MYSQL *dbCon, const int userId, char *importedFile) {
    int status = EXIT_FAILURE;
    CredsArray *credsArray = parseImportCredsList(importedFile);

    if (credsArray != NULL) {
        for (unsigned int i = 0; i < credsArray->size; i++) {
            if (credsArray->credentials != NULL) {
                credsArray->credentials[i].userId = userId;
                status = createNewCreds(dbCon, &credsArray->credentials[i]);
            }
        }

        freeCredsArray(credsArray);
        if (credsArray != NULL) free(credsArray);
    }
    
    return status;
}

int exportPasswordsController(MYSQL *dbCon, const int userId, char *exportFolder) {
    ExportList exportedList = getPasswordsExportListDb(dbCon, userId);

    int status = writePasswordsExportFile(exportedList.lines, exportedList.count, exportFolder);

    return status;
}


void freeExportList(ExportList *exportList) {
    for (unsigned int i = 0; i < exportList->count; i++) {
        free(exportList->lines[i]);
    }
    free(exportList->lines);
    exportList->lines = NULL;
    exportList->count = 0;
}