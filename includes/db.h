#ifndef _DB_HEADS_FILE
#define _DB_HEADS_FILE

#ifdef __linux__
#include <mysql/mysql.h>
#elif _WIN32
#include <mysql.h>
#endif

MYSQL *dbConnect();
void closeDb(MYSQL *dbCon);
int dbGet(MYSQL *, char *);
int putData(MYSQL *dbCon, char *sqlQuery);

#endif