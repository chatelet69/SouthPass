#include <stdio.h>
#include <stdlib.h>
#include "../../includes/db.h"

// Fonction de récupération 
int getData(MYSQL *dbCon, char *sqlQuery) {      
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

int putData(MYSQL *dbCon, char *sqlQuery) {
    if (mysql_query(dbCon, sqlQuery) != 0) {
        fprintf(stderr, "Query Failure\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}