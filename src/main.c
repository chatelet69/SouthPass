#include <stdio.h>
#include <stdlib.h>
#include "../includes/pincludes.h"

int main(int argc, char **argv)
{
    int status = EXIT_SUCCESS;
    MYSQL *db = dbConnect();
    //int resDb = dbGet(db, "SELECT * FROM users;");

    //char *username;
    //username = malloc(41 * sizeof(char));

    /*do {
        printf("Username : ");
        fgets(username, 40, stdin);
        removeReturnLine(username);
    } while (strcmp(username, "") == 0);

    char *query;
    query = malloc(sizeof("INSERT INTO users (username) VALUES ('%s')", username));
    sprintf(query, "INSERT INTO users (username) VALUES ('%s')", username);
    resDb = putData(db, query);*/

    status = showGuiApp(argc, argv);
    closeDb(db);

    return status;
}
