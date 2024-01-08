#ifndef DB_H_FILE
#define DB_H_FILE

#ifdef __linux__
#include <mysql/mysql.h>
#elif _WIN32
#include <mysql.h>
#endif

#include "../includes/models.h"

MYSQL *dbConnect();
void closeDb(MYSQL *dbCon);

int dbGet(MYSQL *, char *);
int putData(MYSQL *dbCon, char *sqlQuery);

// CRUD 
int isUserExist(MYSQL *, char *);
int createUser(MYSQL *, char *, char *, char *);
CredsArray getPasswordsList(MYSQL *dbCon, int userId);
int createNewCreds(MYSQL *dbCon, Credentials *creds);
char *shaPwd(const char *, char *, char *);
int verifLogin(MYSQL *, char *, char *, char *);
char *getSaltByEmail(MYSQL *, char *);
int getUserByTokenInfos(MYSQL *dbCon, const char *token, const int userId);

#endif