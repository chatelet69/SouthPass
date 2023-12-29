#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../includes/db.h"
#include "../../includes/utils.h"

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

CredsArray getPasswordsList(MYSQL *dbCon, int userId) {
    CredsArray credsArray;
    credsArray.size = 0;
    credsArray.creds = NULL;
    const char *sqlQuery = "SELECT id,name,loginName,password FROM pswd_stock WHERE userId = 1";
    
    if (mysql_query(dbCon, sqlQuery) != 0) {
        fprintf(stderr, "Query Failure\n");
        return credsArray;
    } else {
        MYSQL_RES *resData = mysql_store_result(dbCon);
        if (resData == NULL) {
            fprintf(stderr, "No data\n");
        } else {
            char numFields = mysql_num_fields(resData);
            unsigned int rowsCount = mysql_num_rows(resData);
            MYSQL_ROW row;

            if (rowsCount > 0) {
                int cred = 0;
                credsArray.creds = (Credentials *) malloc(sizeof(Credentials) * rowsCount);
                credsArray.size = rowsCount;
                if (credsArray.creds != NULL) {
                    while ((row = mysql_fetch_row(resData))) {
                        credsArray.creds[cred].id = atoi(row[0]);
                        credsArray.creds[cred].name = strdup(row[1]);
                        credsArray.creds[cred].loginName = strdup(row[2]);
                        credsArray.creds[cred].password = strdup(row[3]);
                        //for(int i = 0; i < numFields; i++) printf("| %s |", row[i] ? row[i] : "NULL");
                        printf("\n");
                        cred++;
                    }
                }
            }
            mysql_free_result(resData);
        }
    }
    
    return credsArray;
}

int createNewCreds(MYSQL *dbCon, Credentials creds) {
    if (creds.id == 0) return EXIT_FAILURE;
    int initialSize = sizeof("INSERT INTO pswd_stock (userId,name,loginName,password) VALUES ()");
    int userIdSize = getSizeInString(creds.id);
    int totalSize = initialSize + (sizeof(creds.loginName) + sizeof(creds.name) + sizeof(creds.password) + userIdSize);
    char *sqlQuery = (char *) malloc(totalSize * sizeof(char));

    sprintf(sqlQuery, 
            "INSERT INTO pswd_stock (userId,name,loginName,password) VALUES (%d, '%s', '%s', '%s');", 
            creds.id, creds.name, creds.loginName, creds.password);

    if (mysql_query(dbCon, sqlQuery) != 0) {
        fprintf(stderr, "Query Failure\n");
        return EXIT_FAILURE;
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