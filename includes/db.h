#ifndef DB_H_FILE
#define DB_H_FILE

#ifdef __linux__
#include <mysql/mysql.h>
#elif _WIN32
#include <mysql.h>
#endif

#include "models.h"

#define SUCCESS_CODE 1
#define FAIL_CODE 2

#define SUCCESS_CODE 1
#define FAIL_CODE 2

MYSQL *dbConnect();
void closeDb(MYSQL *dbCon);

int dbGet(MYSQL *, char *);
int putData(MYSQL *dbCon, char *sqlQuery);

// CRUD 
int isUserExist(MYSQL *, char *);
int createUser(MYSQL *, char *, char *, char *);
CredsArray *getPasswordsList(MYSQL *dbCon, int userId);
int createNewCreds(MYSQL *dbCon, Credentials *creds);
char * checkLoginDb(MYSQL *dbCon, char *, char *, char *);
const char *getSaltByEmail(MYSQL *mysql, char *email);
int getUserByTokenInfos(MYSQL *dbCon, const char *token, const int userId);
int saveNewUserTokenDb(MYSQL *dbCon, const int userId, char *tokenHash);
int getUserIdBy(MYSQL *dbCon, char *search, char *searchOption);
ExportList getPasswordsExportListDb(MYSQL *dbCon, const int userId);
struct PwdList *getUniquePwd(MYSQL *dbCon, int userId);
struct WebsiteByPwd * getWebsiteByPwd(MYSQL * dbCon, char * pwd, int id);
LoginsList *getUniquesLoginsById(MYSQL *dbCon, const int userId);

#endif