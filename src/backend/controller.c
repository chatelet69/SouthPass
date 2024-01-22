/*
    Filename : controller.c
    Description : Main Backend controller (interact with Front and Database)
    Last Edit : 21_01_2024
*/

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
#include "../../includes/backLoginSignIn.h"
#include "../../includes/fileController.h"

void freeCredsArray(struct CredsArray *credsArray) {
    if (credsArray != NULL) {
        for (unsigned int i = 0; i < credsArray->size; i++) {
            free(credsArray->credentials[i].name);
            free(credsArray->credentials[i].loginName);
            free(credsArray->credentials[i].password);
        }
        if (credsArray->credentials != NULL) free(credsArray->credentials);
        credsArray->size = 0;
    }
}

void freeCredentialsData(Credentials *creds) {
    if (creds != NULL) {
        if (creds->name != NULL) free(creds->name);
        if (creds->loginName != NULL) free(creds->loginName);
        if (creds->password != NULL) free(creds->password);
        free(creds);
    }
}

void freeToken(TokenInfos *token) {
    if (token != NULL) {
        if (token->email) free(token->email);
        if (token->token) free(token->token);
        free(token);
    }
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
    int nameSize = strlen(name);
    int loginSize = strlen(loginName);
    int passwordSize = strlen(password);

    if (nameSize == 0 || nameSize > CRED_NAME_MAX_SIZE)
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

int generateNewUserToken(MYSQL *dbCon, char *userEmail, char *hashPassword, int userId) {
    char emailOption[6];
    strcpy(emailOption, "email");
    int freshUserId = userId;
    if (userId == 0) freshUserId = getUserIdBy(dbCon, userEmail, emailOption);

    if (freshUserId != 0) {
        char *actualDateStr = getActualDate();
        char *baseToken = (char *) malloc(sizeof(char) * 130);
        srand(time(NULL));
        int randomVal = rand() % (2000) + 1000;
        sprintf(baseToken, "%s_%d_%s_%d_%s", userEmail, freshUserId, actualDateStr, randomVal, hashPassword);
        char tokenHash[65];

        char *hashString = (char*)malloc(2*SHA256_DIGEST_LENGTH+1);
        strcpy(tokenHash, shaPwd(baseToken, hashString, actualDateStr));
        
        free(hashString);
        free(baseToken);
        free(actualDateStr);

        saveNewUserTokenDb(dbCon, freshUserId, tokenHash);
        saveNewTokenFile(tokenHash, userEmail, freshUserId);

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

CredsArray *searchCredsBy(MYSQL *dbCon, int userId, char *searchValue, const char *searchType) {
    if (searchValue != NULL) {
        if (strlen(searchValue) > 255 || strlen(searchValue) == 0) return NULL;
        char *finalSearch = (char *) malloc(sizeof(char) * (strlen(searchValue) + 5));
        sprintf(finalSearch, "%s%%", searchValue);

        CredsArray *credsArray = getPasswordsListBy(dbCon, userId, finalSearch, searchType);
        free(finalSearch);

        return credsArray;
    }

    return NULL;
}

int deleteCredentialController(MYSQL *dbCon, int credentialId, int userId) {
    if (credentialId == 0 || userId == 0) return EXIT_FAILURE;

    int resStatus = deleteCredentialDb(dbCon, credentialId, userId);

    return resStatus;
}

int checkCredentialsData(struct Credentials *credentials) {
    if (credentials == NULL) return EXIT_FAILURE;
    if (credentials->id == 0 || credentials->userId == 0) return EXIT_FAILURE;
    
    if (credentials->name == NULL || credentials->loginName == NULL || 
        credentials->password == NULL) return EXIT_FAILURE;
    if (strlen(credentials->name) == 0 || strlen(credentials->name) > CRED_NAME_MAX_SIZE) return EXIT_FAILURE;
    if (strlen(credentials->loginName) == 0 || strlen(credentials->loginName) > LOGIN_MAX_SIZE) return EXIT_FAILURE;
    if (strlen(credentials->password) == 0 || strlen(credentials->password) > PASSWORD_MAX_SIZE) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int saveEditedCredsController(MYSQL *dbCon, const int credId, const int userId, const char *name, const char *login, const char *password) {
    Credentials *credentials = (Credentials *) malloc(sizeof(Credentials));
    if (credentials == NULL) return EXIT_FAILURE;

    credentials->id = credId;
    credentials->userId = userId;
    credentials->name = strdup(name);
    credentials->loginName = strdup(login);
    credentials->password = strdup(password);

    int status = checkCredentialsData(credentials);
    if (status == EXIT_SUCCESS) status = saveEditedCredsDb(dbCon, credentials);
    freeCredentialsData(credentials);

    return status;
}

int saveEditedEmail(MYSQL *dbCon, int userId, char *newEmail, char *actualEmail) {
    if (strcmp(newEmail, actualEmail) == 0 || userId == 0 || strlen(newEmail) > 255) {
        return EXIT_FAILURE;
    }

    int status = saveNewEmailDb(dbCon, userId, newEmail);
    if (status == EXIT_SUCCESS) {
        // On met à jour le fichier de token en inscrivant le nouveau mail
        TokenInfos *tokenInfos = getTokenFileInfos();
        saveNewTokenFile(tokenInfos->token, newEmail, tokenInfos->id);
        freeToken(tokenInfos);
    }

    return status;
}

int saveEditedPwdAccount(MYSQL *dbCon, int userId, char *newPassword, char *actualPass, char *passwordType) {
    int status = EXIT_FAILURE;
    // Vérification du mot de passe (taille, format, ce qu'il contient)
    if (strcmp(newPassword, actualPass) == 0 || userId == 0) return -3;
    if (!hasDigit(newPassword)) return -1;
    if (!hasSpecialChar(newPassword)) return -1;
    if (strlen(newPassword) < 10 || strlen(newPassword) > 60) return -1;

    /*
    if(verifPasswordChars(pwd) == 0 || strlen(pwd)<10)
        return 1;
    if(verifPasswordChars(verifPwd) == 0 || strlen(verifPwd)<10)
        return 1;
    if(strcmp(pwd, verifPwd) != 0)
        return 2;*/

    // Récupération du mail (dernier en date en db) et salt de l'utilisateur
    char *email = getEmailByUserId(dbCon, userId);
    if (email == NULL) return EXIT_FAILURE;
    char salt[6];
    strcpy(salt, getSaltByEmail(dbCon, email));

    char actualHash[65];
    char* actualHashString = (char*) malloc(2*SHA256_DIGEST_LENGTH+1);
    strcpy(actualHash, shaPwd(actualPass, actualHashString, salt));
    free(actualHashString);

    char hashPassword[65];
    char* hashString = (char*) malloc(2*SHA256_DIGEST_LENGTH+1);
    strcpy(hashPassword, shaPwd(newPassword, hashString, salt));
    free(hashString);

    int checkPwdId = checkPwdBy(dbCon, userId, passwordType, actualHash);
    if (checkPwdId == userId) {
        // Appel DB avec le type de mot de passe à update
        status = saveNewAccountPasswordDb(dbCon, userId, hashPassword, passwordType);
        if (status == EXIT_SUCCESS && strcmp(passwordType, "pwdMaster") == 0) {
            // Mise à jour des mots de passes (nouvelle clé de chiffrement)
            char *hash = strdup(actualHash);
            status = updateAllPasswords(dbCon, userId, hash);
            free(hash);
        }
        if (status == EXIT_SUCCESS && strcmp(passwordType, "pwdAccount") == 0) {
            // Récupération de l'email et réecriture du token
            generateNewUserToken(dbCon, email, hashPassword, userId);
        }
    } else {
        status = -2;
    }
    free(email);

    return status;
}