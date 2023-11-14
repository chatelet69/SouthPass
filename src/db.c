/*
    Filename:   db.c
    Creation:   10/11
    Description:    Définit les fonctions d'utilisation de la base de données
                    avec la bibliothèque libmysql
    Authors;    
*/

#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include "../includes/db.h"

MYSQL *dbConnect() {
    char *host = "localhost";
    char *user = "root";
    char *dbName = "projet_c";
    char *password = "storm";
    MYSQL *dbCon;

    printf("MySQL client version: %s\n", mysql_get_client_info());

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
            int num_fields = mysql_num_fields(resData);
            MYSQL_ROW row;
        
            printf(" ID     Nom\n");
            while ((row = mysql_fetch_row(resData))) {
                for(int i = 0; i < num_fields; i++) printf("| %s |", row[i] ? row[i] : "NULL");
                printf("\n");
            }
            printf("______________\n");
        }
        mysql_free_result(resData);
    }
    
    return EXIT_SUCCESS;
}