#ifndef CONTROLLER_H
#define CONTROLLER_H

#define CRED_NAME_MAX_SIZE 150
#define LOGIN_MAX_SIZE 255
#define PASSWORD_MAX_SIZE 40

#define LEAKCHECK_BASE_URL "https://leakcheck.io/api/public"

#include "config_db.h"
#include "./models.h"

void printCreds(Credentials *creds, unsigned int size);
void freeCredsArray(struct CredsArray *credsArray);
void freeCredentialsData(Credentials *creds);
void freeExportList(ExportList *exportList);
void freeToken(TokenInfos *token);
int getUserIdByToken(MYSQL *dbCon);
int checkCredentialsData(Credentials *credentials);

char *getActualDate();
int generateNewUserToken(MYSQL *dbCon, char *userEmail, char *hashPassword, int userId);
int addNewCredsController(MYSQL *dbCon, char *loginName, char *login, char *password);
int exportPasswordsController(MYSQL *dbCon, const int userId, char *exportFolder);
int importPasswordsController(MYSQL *dbCon, const int userId, char *importedFile);
int deleteCredentialController(MYSQL *dbCon, int credentialId, int userId);
int saveEditedCredsController(MYSQL *dbCon, const int credId, const int userId, const char *name, const char *login, const char *password);
int saveEditedEmail(MYSQL *dbCon, int userId, char *newEmail, char *actualEmail);
int saveEditedPwdAccount(MYSQL *dbCon, int userId, char *newPassword, char *actualPass, char *passwordType);

// API
void printLeaksList(LeaksList *list);
void freeLeaksList(struct LeaksList *list);
LeaksList *getDataLeaksFromLeakCheck(MYSQL *dbCon, const int userId);
CredsArray *searchCredsBy(MYSQL *dbCon, int userId, char *searchValue, const char *searchType);
int getVerifCode(char *email);

#endif