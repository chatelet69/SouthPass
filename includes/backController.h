#ifndef CONTROLLER_H
#define CONTROLLER_H

#define LOGIN_NAME_MAX_SIZE 150
#define LOGIN_MAX_SIZE 255
#define PASSWORD_MAX_SIZE 40

#include "./models.h"

char **getPwsdList();
void printCreds(Credentials *creds, unsigned int size);
void freeCredsArray(CredsArray credsArray);
void printPCred(Credentials *creds);
void freeCredentialsData(Credentials *creds);
int getUserIdByToken(MYSQL *dbCon);

int addNewCredsController(MYSQL *dbCon,
                          char *loginName, 
                          char *login, 
                          char *password);

#endif