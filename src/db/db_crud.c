#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "../../includes/utils.h"
#include "../../includes/db.h"
#include "../../includes/models.h"
#include "../../includes/pincludes.h"
#include <openssl/sha.h>

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

CredsArray *getPasswordsList(MYSQL *dbCon, int userId) {
    CredsArray *credsArray = (CredsArray *) malloc(sizeof(CredsArray) * 1);
    credsArray->size = 0;
    credsArray->credentials = NULL;
    char *sqlQuery = (char *) malloc(sizeof(char) * 256);
    
    if (sqlQuery == NULL) return NULL;
    sprintf(sqlQuery, "SELECT psw.id AS id,userId,name,loginName,AES_DECRYPT(psw.password, UNHEX(u.pwdMaster)) AS password FROM pswd_stock psw INNER JOIN users u ON u.id = psw.userId WHERE userId = %d", userId);
    
    if (mysql_query(dbCon, sqlQuery) != 0) {
        fprintf(stderr, "Query Failure\n");
        return credsArray;
    } else {
        MYSQL_RES *resData = mysql_store_result(dbCon);
        if (resData != NULL) {
            //unsigned int numFields = mysql_num_fields(resData);
            unsigned int rowsCount = mysql_num_rows(resData);
            MYSQL_ROW row;

            if (rowsCount > 0) {
                int cred = 0;
                credsArray->credentials = (Credentials *) malloc(sizeof(Credentials) * rowsCount);
                credsArray->size = rowsCount;    
                if (credsArray->credentials != NULL) {
                    while ((row = mysql_fetch_row(resData))) {
                        credsArray->credentials[cred].id = atoi(row[0]);
                        credsArray->credentials[cred].userId = atoi(row[1]);
                        credsArray->credentials[cred].name = (row[2] ? strdup(row[2]) : strdup("Inconnu"));
                        credsArray->credentials[cred].loginName = (row[3] ? strdup(row[3]) : strdup("Inconnu"));
                        credsArray->credentials[cred].password = (row[4] ? strdup(row[4]) : strdup("Inconnu"));
                        cred++;
                    }
                }
            }
            mysql_free_result(resData);
        }
    }
    free(sqlQuery);
    
    return credsArray;
}

int createNewCreds(MYSQL *dbCon, Credentials *creds) {
    int status = EXIT_FAILURE;
    if (creds->userId == 0) return status;
    const char *sqlQuery = "INSERT INTO pswd_stock (userId,name,loginName,password) VALUES (?,?,?,AES_ENCRYPT(?,(SELECT UNHEX(pwdMaster) FROM users WHERE id = ?)))";

    MYSQL_STMT *stmt = mysql_stmt_init(dbCon);
    if (mysql_stmt_prepare(stmt, sqlQuery, strlen(sqlQuery)) == 0) {
        MYSQL_BIND params[5];
        memset(params, 0, sizeof(params));
        params[0].buffer_type = MYSQL_TYPE_LONG;    
        params[0].buffer = &creds->userId;

        params[1].buffer_type = MYSQL_TYPE_VARCHAR;
        params[1].buffer = creds->name;
        params[1].buffer_length = strlen(creds->name);

        params[2].buffer_type = MYSQL_TYPE_VARCHAR;
        params[2].buffer = creds->loginName;
        params[2].buffer_length = strlen(creds->loginName);
        
        params[3].buffer_type = MYSQL_TYPE_VARCHAR;
        params[3].buffer = creds->password;
        params[3].buffer_length = strlen(creds->password);
        
        params[4].buffer_type = MYSQL_TYPE_LONG;
        params[4].buffer = &creds->userId;
        
        if (mysql_stmt_bind_param(stmt, params)) {
            mysql_stmt_close(stmt);
            return status;
        }
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
            return 1; // user exist
        }

        mysql_stmt_close(stmt);
        return 0; // dont exist
    }else{
        mysql_stmt_close(stmt);
        return 2;
    }
}

int createUser(MYSQL *dbCon, char * email, char * pwd, char *masterPwd){
    // hashage du password
    char hashedPwd[65];
    char* hashString = (char*)malloc(2*SHA256_DIGEST_LENGTH+1);
    int salt2; // sel généré
    srand(time(NULL));
    salt2 = rand()%99999;
    char salt[6]; // sel transformé en string
    sprintf(salt, "%d", salt2);
    strcpy(hashedPwd, shaPwd(pwd, hashString, salt)); // on hash le pwd salé et on le met dans hashedPwd
    free(hashString);

    // hashage du pwd maitre avec le meme sel
    char hashedMasterPwd[65];
    char* hashMasterString = (char*)malloc(2*SHA256_DIGEST_LENGTH+1);
    strcpy(hashedMasterPwd, shaPwd(masterPwd, hashMasterString, salt));
    free(hashMasterString);

    printf("\nhashsign : %s , hashmastersign %s , sel : %s\n", hashedPwd, hashedMasterPwd, salt);
    // verif si le user existe déjà (via l'email)
    int res = 0;

    res = isUserExist(dbCon, email);
    if(res == 1){
        return 2;
    }else if(res==2){
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
    generateNewUserToken(dbCon, email);
    return status;
}

char * checkLoginDb(MYSQL *dbCon, char *email, char *hashedPwd, char *hashedMasterPwd) {
    char * ko = "ko";
    MYSQL_STMT    *stmt;
    MYSQL_BIND    bind[1];
    MYSQL_RES     *prepare_meta_result;
    MYSQL_TIME    ts;
    unsigned long length[1];
    bool          is_null[1];
    bool          error[1];
    char verifEmail[255];
    const char *sqlQuery = "SELECT email FROM users WHERE email = ? AND pwdAccount = ? AND pwdMaster = ?";

    stmt = mysql_stmt_init(dbCon);
    mysql_stmt_prepare(stmt, sqlQuery, strlen(sqlQuery));
    MYSQL_BIND params[3];
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
    mysql_stmt_bind_param(stmt, params);

    mysql_stmt_execute(stmt);

    prepare_meta_result = mysql_stmt_result_metadata(stmt);
    memset(bind, 0, sizeof(bind));
    bind[0].buffer_type= MYSQL_TYPE_STRING;
    bind[0].buffer= (char *)verifEmail;
    bind[0].buffer_length= 255;
    bind[0].is_null= &is_null[0];
    bind[0].length= &length[0];
    bind[0].error= &error[0];

    mysql_stmt_bind_result(stmt, bind);
    mysql_stmt_store_result(stmt);
    mysql_stmt_fetch(stmt);


    if (is_null[0]){
        mysql_free_result(prepare_meta_result);
        mysql_stmt_close(stmt);
        return ko;
    }
    mysql_free_result(prepare_meta_result);
    mysql_stmt_close(stmt);
    return strdup(verifEmail);
}

int getUserByTokenInfos(MYSQL *dbCon, const char *token, const int userId) {
    int status = 0;
    printf("%d : %s\n", userId, token);
    const char *sqlQuery = "SELECT id FROM users WHERE token = ? AND id = ?";

    MYSQL_STMT *stmt = mysql_stmt_init(dbCon);

    if (mysql_stmt_prepare(stmt, sqlQuery, strlen(sqlQuery)) == 0) {
        MYSQL_BIND params[2];
        memset(params, 0, sizeof(params));
        params[0].buffer_type = MYSQL_TYPE_VARCHAR;
        params[0].buffer = (void *)token;
        params[0].buffer_length = strlen(token);
        params[1].buffer_type = MYSQL_TYPE_LONG;
        params[1].buffer = (void *)&userId;

        if (mysql_stmt_bind_param(stmt, params)) {
            mysql_stmt_close(stmt);
            return status;
        }

        if (mysql_stmt_execute(stmt)) {
            mysql_stmt_close(stmt);
            return status;
        }
        
        int rowsCount = 0;
        while (mysql_stmt_fetch(stmt) == 0) rowsCount++;
        
        if (rowsCount == 1) status = 1;
    }
    mysql_stmt_close(stmt);
    
    return status;
}

const char *getSaltByEmail(MYSQL *mysql, char *email) {
    const char * ko = "ko";
    char salt[7];
    MYSQL_STMT    *stmt;
    MYSQL_BIND    bind[1];
    MYSQL_RES     *prepare_meta_result;
    MYSQL_TIME    ts;
    unsigned long length[1];
    bool          is_null[1];
    bool          error[1];

    stmt = mysql_stmt_init(mysql);
    mysql_stmt_prepare(stmt, "SELECT salt FROM users WHERE email = ?", strlen("SELECT salt FROM users WHERE email = ?"));
    MYSQL_BIND params[1];
    memset(params, 0, sizeof(params));
    params[0].buffer_type = MYSQL_TYPE_VARCHAR;
    params[0].buffer = email;
    params[0].buffer_length = strlen(email);
    mysql_stmt_bind_param(stmt, params);

    mysql_stmt_execute(stmt);
    prepare_meta_result = mysql_stmt_result_metadata(stmt);
    memset(bind, 0, sizeof(bind));
    bind[0].buffer_type= MYSQL_TYPE_STRING;
    bind[0].buffer= (char *)salt;
    bind[0].buffer_length= sizeof(salt);
    bind[0].is_null= &is_null[0];
    bind[0].length= &length[0];
    bind[0].error= &error[0];

    mysql_stmt_bind_result(stmt, bind);
    mysql_stmt_store_result(stmt);
    mysql_stmt_fetch(stmt);
        if (is_null[0]){
            fprintf(stdout, " NULL\n");
            return ko;
        }
    mysql_free_result(prepare_meta_result);
    mysql_stmt_close(stmt);
    return strdup(salt);
}

int saveNewUserTokenDb(MYSQL *dbCon, const int userId, char *tokenHash) {
    int status = EXIT_FAILURE;
    if (userId == 0) return status;
    const char *sqlQuery = "UPDATE users SET token = ? WHERE id = ?";

    MYSQL_STMT *stmt = mysql_stmt_init(dbCon);
    if (mysql_stmt_prepare(stmt, sqlQuery, strlen(sqlQuery)) == 0) {
        MYSQL_BIND params[2];
        memset(params, 0, sizeof(params));
        params[0].buffer_type = MYSQL_TYPE_VARCHAR;
        params[0].buffer = tokenHash;
        params[0].buffer_length = strlen(tokenHash);

        params[1].buffer_type = MYSQL_TYPE_LONG;
        params[1].buffer = (void *) &userId;

        if (mysql_stmt_bind_param(stmt, params)) {
            fprintf(stderr, "Error mysql saveNewUserTokenDb\n");
            mysql_stmt_close(stmt);
            return status;
        }
        status = mysql_stmt_execute(stmt);
    }
    mysql_stmt_close(stmt);

    return status;
}

int getUserIdBy(MYSQL *dbCon, char *search, char *searchOption) {
    int userId = 0;
    const char *baseQuery = "SELECT id FROM users WHERE somethingmaybelong = something";
    char *sqlQuery = (char *) malloc(sizeof(char) * strlen(baseQuery));
    sprintf(sqlQuery, "SELECT id FROM users WHERE %s = ?", searchOption);

    MYSQL_STMT *stmt = mysql_stmt_init(dbCon);
    if (mysql_stmt_prepare(stmt, sqlQuery, strlen(sqlQuery)) == 0) {
        MYSQL_BIND params[1];
        memset(params, 0, sizeof(params));
        params[0].buffer_type = MYSQL_TYPE_VARCHAR;
        params[0].buffer = search;
        params[0].buffer_length = strlen(search);

        if (mysql_stmt_bind_param(stmt, params) != EXIT_SUCCESS) {
            mysql_stmt_close(stmt);
            return userId;
        }
        int status = mysql_stmt_execute(stmt);

        if (status == EXIT_SUCCESS) {
            int tmpId = 0;
            MYSQL_BIND results[1];
            memset(results, 0, sizeof(results));
            results[0].buffer_type = MYSQL_TYPE_LONG;
            results[0].buffer = &tmpId;
            
            if (mysql_stmt_bind_result(stmt, results) != EXIT_SUCCESS) {
                mysql_stmt_close(stmt);
                return userId;
            }

            if (mysql_stmt_fetch(stmt) == 0) {
                userId = (tmpId > 0) ? tmpId : 0;
            }
        }
    }
    mysql_stmt_close(stmt);

    return userId;
}

ExportList getPasswordsExportListDb(MYSQL *dbCon, const int userId) {
    const char *sqlQuery = "SELECT CONCAT(p.name, CONCAT(',', CONCAT(p.loginName, CONCAT(',', AES_DECRYPT(p.password, UNHEX(u.pwdMaster)))))) AS fullLine FROM pswd_stock p INNER JOIN users u ON p.userId = u.id WHERE p.userId = ?";
    ExportList exportList;
    exportList.count = 0;
    exportList.lines = NULL;

    MYSQL_STMT *stmt = mysql_stmt_init(dbCon);
    MYSQL_RES *metaData;
    if (mysql_stmt_prepare(stmt, sqlQuery, strlen(sqlQuery)) == 0) {
        MYSQL_BIND params[1];
        memset(params, 0, sizeof(params));
        params[0].buffer_type = MYSQL_TYPE_LONG;
        params[0].buffer = (void *) &userId;

        if (mysql_stmt_bind_param(stmt, params) != EXIT_SUCCESS) {
            mysql_stmt_close(stmt);
            //fprintf(stderr, "1 Error db : %s\n", mysql_stmt_error(stmt));
            return exportList;
        }

        int status = mysql_stmt_execute(stmt);
        metaData = mysql_stmt_result_metadata(stmt); 
        mysql_stmt_store_result(stmt);
        if (metaData == NULL) return exportList;

        if (status == EXIT_SUCCESS) {
            char actualLine[300];
            MYSQL_BIND results[1];
            memset(results, 0, sizeof(results));
            results[0].buffer_type = MYSQL_TYPE_STRING;
            results[0].buffer = &actualLine;
            results[0].buffer_length = sizeof(actualLine);
            
            if (mysql_stmt_bind_result(stmt, results) != EXIT_SUCCESS) {
                mysql_stmt_close(stmt);
                return exportList;
            }

            unsigned int rowsCount = mysql_stmt_affected_rows(stmt);
            exportList.lines = (char **) malloc(sizeof(char *) * rowsCount);
                
            int i = 0;
            while (mysql_stmt_fetch(stmt) == 0) {
                char *tmp = strdup(actualLine);
                exportList.lines[i] = (char *) malloc(sizeof(char) * strlen(tmp));
                strcpy(exportList.lines[i], tmp);
                i++;
            }
            exportList.count = i;
        }
        mysql_free_result(metaData);
    }
    mysql_stmt_close(stmt);

    return exportList;
}

LoginsList *getUniquesLoginsById(MYSQL *dbCon, const int userId) {
    const char *sqlQuery = "SELECT DISTINCT loginName FROM pswd_stock WHERE userId = ?";
    LoginsList *loginsList = NULL;

    MYSQL_STMT *stmt = mysql_stmt_init(dbCon);
    MYSQL_RES *metaData;
    if (mysql_stmt_prepare(stmt, sqlQuery, strlen(sqlQuery)) == 0) {
        MYSQL_BIND params[1];
        memset(params, 0, sizeof(params));
        params[0].buffer_type = MYSQL_TYPE_LONG;
        params[0].buffer = (void *) &userId;

        if (mysql_stmt_bind_param(stmt, params) != EXIT_SUCCESS) {
            mysql_stmt_close(stmt);
            //fprintf(stderr, "1 Error db : %s\n", mysql_stmt_error(stmt));
            return loginsList;
        }

        int status = mysql_stmt_execute(stmt);
        metaData = mysql_stmt_result_metadata(stmt); 
        mysql_stmt_store_result(stmt);
        if (metaData == NULL) return loginsList;

        if (status == EXIT_SUCCESS) {
            loginsList = (LoginsList *) malloc(sizeof(LoginsList));
            loginsList->count = 0;

            char actualLine[255];
            MYSQL_BIND results[1];
            memset(results, 0, sizeof(results));
            results[0].buffer_type = MYSQL_TYPE_STRING;
            results[0].buffer = &actualLine;
            results[0].buffer_length = sizeof(actualLine);
            
            if (mysql_stmt_bind_result(stmt, results) != EXIT_SUCCESS) {
                mysql_stmt_close(stmt);
                return loginsList;
            }

            unsigned int rowsCount = mysql_stmt_affected_rows(stmt);
            loginsList->logins = (char **) malloc(sizeof(char *) * rowsCount);
                
            int i = 0;
            while (mysql_stmt_fetch(stmt) == 0) {
                char *tmp = strdup(actualLine);
                // On ajoute chaque login à la liste
                loginsList->logins[i] = (char *) malloc(sizeof(char) * strlen(tmp));
                strcpy(loginsList->logins[i], tmp);
                i++;
            }
            loginsList->count = i;
        }
        mysql_free_result(metaData);
    }
    mysql_stmt_close(stmt);

    return loginsList;
}