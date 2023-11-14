#ifndef _db_heads_file
#define _db_heads_file

#include <mysql.h>

MYSQL *dbConnect();
void closeDb(MYSQL *);
int dbGet(MYSQL *, char *);

#endif