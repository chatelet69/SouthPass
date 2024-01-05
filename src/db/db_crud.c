#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../includes/db.h"
#include <openssl/sha.h>
#include <time.h>
#include <stdint.h>
#include <mysql.h>
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
    const char *sqlQuery = "SELECT id,userId,name,loginName,password FROM pswd_stock WHERE userId = 1";
    
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
                        credsArray.creds[cred].userId = atoi(row[1]);
                        credsArray.creds[cred].name = strdup(row[2]);
                        credsArray.creds[cred].loginName = strdup(row[3]);
                        credsArray.creds[cred].password = strdup(row[4]);
                        //for(int i = 0; i < numFields; i++) printf("| %s |", row[i] ? row[i] : "NULL");
                        //printf("\n");
                        cred++;
                    }
                }
            }
            mysql_free_result(resData);
        }
    }
    
    return credsArray;
}

int createNewCreds(MYSQL *dbCon, Credentials *creds) {
    int status = EXIT_FAILURE;
    if (creds->userId == 0) return status;
    int size = strlen("INSERT INTO pswd_stock (userId,name,loginName,password) VALUES (?,?,?,?)");

    MYSQL_STMT *stmt = mysql_stmt_init(dbCon);
    if (mysql_stmt_prepare(stmt, "INSERT INTO pswd_stock (userId,name,loginName,password) VALUES (?,?,?,?)", size) == 0) {
        MYSQL_BIND params[4];
        memset(params, 0, sizeof(params));
        params[0].buffer_type = MYSQL_TYPE_LONG;
        params[0].buffer = &creds->userId;

        params[1].buffer_type = MYSQL_TYPE_VARCHAR;
        params[1].buffer = creds->name;
        params[1].buffer_length = sizeof(creds->name);

        params[2].buffer_type = MYSQL_TYPE_VARCHAR;
        params[2].buffer = creds->loginName;
        params[2].buffer_length = sizeof(creds->loginName);
        
        params[3].buffer_type = MYSQL_TYPE_VARCHAR;
        params[3].buffer = creds->password;
        params[3].buffer_length = sizeof(creds->password);
        mysql_stmt_bind_param(stmt, params);
        status = mysql_stmt_execute(stmt);
    }
    mysql_stmt_close(stmt);

    return status;
}

int putData(MYSQL *dbCon, char *sqlQuery) {
    if (mysql_query(dbCon, sqlQuery) != 0) {
        fprintf(stderr, "Query Failure\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int isUserExist(MYSQL *dbCon, char * email) {
    int status = 0;
    const char * sqlQuery = "SELECT email FROM users WHERE email = ?";

    int size = strlen(sqlQuery);

    MYSQL_STMT *stmt = mysql_stmt_init(dbCon);
    if (mysql_stmt_prepare(stmt, sqlQuery, size) == 0) {
        MYSQL_BIND params[1];
        memset(params, 0, sizeof(params));
        params[0].buffer_type = MYSQL_TYPE_VARCHAR;
        params[0].buffer = email;
        params[0].buffer_length = strlen(email);

        mysql_stmt_bind_param(stmt, params);
        status = mysql_stmt_execute(stmt);

        if(status != 0){
            mysql_stmt_close(stmt);
            return 2;
        }
        status = mysql_stmt_fetch(stmt);
        if (status != 1 && status != MYSQL_NO_DATA){
            mysql_stmt_close(stmt);
            printf("\nd");
            return 1; // user exist
        }
        printf("\nd");

        mysql_stmt_close(stmt);
        return 0; // dont exist
    }else{
        mysql_stmt_close(stmt);
        return 2;
    }
}

int createUser(MYSQL *dbCon, char * email, char * pwd, char *masterPwd){

    // hashage du password
    char hashedPwd[257];
    char* hashString = (char*)malloc(2*SHA256_DIGEST_LENGTH+1);
    int salt2; // sel généré
    srand(time(NULL));
    salt2 = rand()%99999;
    char salt[6]; // sel transformé en string
    sprintf(salt, "%d", salt2);
    strcpy(hashedPwd, shaPwd(pwd, hashString, salt)); // on hash le pwd salé et on le met dans hashedPwd
    free(hashString);

    // hashage du pwd maitre avec le meme sel
    char hashedMasterPwd[257];
    char* hashMasterString = (char*)malloc(2*SHA256_DIGEST_LENGTH+1);
    strcpy(hashedMasterPwd, shaPwd(masterPwd, hashMasterString, salt));
    free(hashMasterString);

    // verif si le user existe déjà (via l'email)
    int res = 0;

    res = isUserExist(dbCon, email);
    printf("\nres : %d", res);
    if(res == 1){
        printf("Erreur, cet user existe");
        return 2;
    }else if(res==2){
        printf("\nErreur lors de la requete SQL");
        return 1;
    }

    int status = EXIT_FAILURE;

    const char * sqlQuery = "INSERT INTO users (email,pwdAccount,pwdMaster,salt) VALUES (?,?,?,?)";
    int size = strlen(sqlQuery);
    MYSQL_STMT *stmt = mysql_stmt_init(dbCon);
    if (mysql_stmt_prepare(stmt, sqlQuery, size) == 0) {
        MYSQL_BIND params[4];
        memset(params, 0, sizeof(params));

        params[0].buffer_type = MYSQL_TYPE_VARCHAR;
        params[0].buffer = email;
        params[0].buffer_length = strlen(email);

        params[1].buffer_type = MYSQL_TYPE_VARCHAR;
        params[1].buffer = hashedPwd;
        params[1].buffer_length = strlen(hashedPwd);

        params[2].buffer_type = MYSQL_TYPE_VARCHAR;
        params[2].buffer = hashedMasterPwd;
        params[2].buffer_length = strlen(hashedMasterPwd);

        params[3].buffer_type = MYSQL_TYPE_VARCHAR;
        params[3].buffer = salt;
        params[3].buffer_length = strlen(salt);

        mysql_stmt_bind_param(stmt, params);
        status = mysql_stmt_execute(stmt);
    }
    mysql_stmt_close(stmt);

    return status;
}

char * shaPwd(const char * pwd, char * hashString, char * salt){
    char saledPwd[62];
    strcpy(saledPwd, salt);
    strcat(saledPwd, pwd);
    strcat(saledPwd, salt);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    const unsigned char* data = (const unsigned char*)saledPwd;

    SHA256(data, strlen(saledPwd), hash);

    // Convertir le hash en chaîne de caractères
    if (hashString == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(&hashString[i * 2], "%02x", hash[i]);
    }
    hashString[2 * SHA256_DIGEST_LENGTH] = '\0'; // Ajouter le caractère nul à la fin de la chaîne

    return hashString;
}