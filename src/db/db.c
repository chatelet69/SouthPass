/*
    Filename:   db.c
    Creation:   10/11
    Description:    Définit les fonctions d'utilisation de la base de données
                    avec la bibliothèque libmysql
    Authors;    
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../includes/db.h"

MYSQL *dbConnect() {
    const char *host = "";
    const char *user = "";
    const char *dbName = "southpass";
    const char *password = "";
    MYSQL *dbCon;

//    printf("MySQL client version: %s\n", mysql_get_client_info());

    if ((dbCon = mysql_init(NULL)) == NULL) {
        fprintf(stderr, "Could not init DB\n");
        return NULL;
    }

    if (mysql_real_connect(dbCon, host, user, password, dbName, 0, NULL, 0) == NULL) {
        fprintf(stderr, "DB Connection Error\n");
        return NULL;
    }

    return dbCon;
}

// CloseDb : clot la connexion
void closeDb(MYSQL *dbCon) {
    mysql_close(dbCon);
}

// Fonction de récupération 
int dbGet(MYSQL *dbCon, char *sqlQuery) {      
    if (mysql_query(dbCon, sqlQuery) != 0) {
        fprintf(stderr, "Query Failure\n");
        return EXIT_FAILURE;
    } else {
        MYSQL_RES *resData = mysql_store_result(dbCon);
        if (resData == NULL) {
            fprintf(stderr, "Aucune data\n");
        } else {
            int numFields = mysql_num_fields(resData);
            MYSQL_ROW row;
            MYSQL_FIELD *column;
            MYSQL_FIELD *fields = mysql_fetch_fields(resData);
        
            for (int col = 0; col < numFields; col++) printf("%s\t", fields[col].name);
            printf("\n\n");

            while ((row = mysql_fetch_row(resData))) {
                for (int col = 0; col < numFields; col++) printf("%s\t", row[col] ? row[col] : "NULL");
                printf("\n");
            }
            printf("______________\n");
        }
        mysql_free_result(resData);
    }
    
    return EXIT_SUCCESS;
}