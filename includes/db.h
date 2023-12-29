#ifndef _DB_HEADS_FILE
#define _DB_HEADS_FILE

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

CredsArray getPasswordsList(MYSQL *dbCon, int userId);
int createNewCreds(MYSQL *dbCon, Credentials creds);

#endif