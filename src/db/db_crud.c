#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <time.h>
#include <stdint.h>
#include "../../includes/utils.h"
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

CredsArray getPasswordsList(MYSQL *dbCon, int userId) {
    CredsArray credsArray;
    credsArray.size = 0;
    credsArray.creds = NULL;
    char *sqlQuery = (char *) malloc(sizeof(char) * 120);
    sprintf(sqlQuery, "SELECT psw.id,userId,name,loginName,AES_DECRYPT(psw.password, u.pwdMaster) FROM pswd_stock psw INNER JOIN users u ON u.id = psw.userId WHERE userId = %d", userId);
    
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
    const char *sqlQuery = "INSERT INTO pswd_stock (userId,name,loginName,password) VALUES (?,?,?,AES_ENCRYPT(?,(SELECT pwdMaster FROM users WHERE id = ?)))";

    MYSQL_STMT *stmt = mysql_stmt_init(dbCon);
    if (mysql_stmt_prepare(stmt, sqlQuery, strlen(sqlQuery)) == 0) {
        MYSQL_BIND params[5];
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
        params[3].buffer_length = strlen(creds->password);
        
        params[4].buffer_type = MYSQL_TYPE_LONG;
        params[4].buffer = &creds->userId;
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

int checkLoginDb(MYSQL *dbCon, char *salt, char *email, char *hashedPwd, char *hashedMasterPwd) {
    int status = 0;
    const char * sqlQuery = "SELECT email FROM users WHERE email = ? AND pwdAccount = ? AND pwdAccount = ?";

    int size = strlen(sqlQuery);

    MYSQL_STMT    *stmt;
    MYSQL_BIND    bind[3];
    MYSQL_RES     *prepare_meta_result;
    int           param_count, column_count, row_count;
    unsigned long length[1];
    bool          is_null[1];
    bool          error[1];

    /* Prepare a SELECT query to fetch data from test_table */
    stmt = mysql_stmt_init(dbCon);
    if (!stmt) {
        fprintf(stderr, " mysql_stmt_init(), out of memory\n");
        return 2;
    }

    if (mysql_stmt_prepare(stmt, sqlQuery, size)) {
        fprintf(stderr, " mysql_stmt_prepare(), SELECT failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        return 2;
    }
    fprintf(stdout, " prepare, SELECT successful\n");
    
    // Get the parameter count from the statement
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
    param_count= mysql_stmt_param_count(stmt);

    fprintf(stdout, " total parameters in SELECT: %d\n", param_count);
    if (param_count != 3) { // validate parameter count
        fprintf(stderr, " invalid parameter count returned by MySQL\n");
        return 1;
    }
    
    /* Execute the SELECT query */
    if (mysql_stmt_execute(stmt)) {
        fprintf(stderr, " mysql_stmt_execute(), failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        return 2;
    }
    
    /* Fetch result set meta information */
    prepare_meta_result = mysql_stmt_result_metadata(stmt);
    if (!prepare_meta_result) {
        fprintf(stderr,
                " mysql_stmt_result_metadata(), returned no meta information\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        return 1;
    }
    
    /* Get total columns in the query */
    column_count= mysql_num_fields(prepare_meta_result);
    fprintf(stdout,
            " total columns in SELECT statement: %d\n",
            column_count);

    if (column_count != 1) /* validate column count */
    {
        fprintf(stderr, " invalid column count returned by MySQL\n");
        return 1;
    }
/* Bind the result buffers for all 4 columns before fetching them */
    memset(bind, 0, sizeof(bind));
    char verifEmail[256];
/* STRING COLUMN */
    bind[0].buffer_type= MYSQL_TYPE_STRING;
    bind[0].buffer= (char *)verifEmail;
    bind[0].buffer_length= strlen(salt);
    bind[0].is_null= &is_null[0];
    bind[0].length= &length[0];
    bind[0].error= &error[0];
/* Bind the result buffers */
    if (mysql_stmt_bind_result(stmt, bind))
    {
        fprintf(stderr, " mysql_stmt_bind_result() failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        return 2;
    }
/* Now buffer all results to client (optional step) */
    if (mysql_stmt_store_result(stmt))
    {
        fprintf(stderr, " mysql_stmt_store_result() failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        return 2;
    }
/* Fetch all rows */
    row_count= 0;
    fprintf(stdout, "Fetching results ...\n");
    mysql_stmt_fetch(stmt);
    row_count++;
    /* column 2 */
    fprintf(stdout, "   column2 (string)   : ");
    if (is_null[0])
        fprintf(stdout, " NULL\n");
    else
        fprintf(stdout, " %s(%ld)\n", salt, length[0]);

    fprintf(stdout, "\n");
/* Validate rows fetched */
    fprintf(stdout, " total rows fetched: %d\n", row_count);
    if (row_count != 1)
    {
        fprintf(stderr, " Mauvais login\n");
        return 1;
    }
/* Free the prepared result metadata */
    mysql_free_result(prepare_meta_result);
/* Close the statement */
    if (mysql_stmt_close(stmt))
    {
        fprintf(stderr, " failed while closing the statement\n");
        fprintf(stderr, " %s\n", mysql_error(dbCon));
    }
    printf("\nFINIT\n");
    return 0;
}

int getUserByTokenInfos(MYSQL *dbCon, const char *token, const int userId) {
    int status = 0;
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

        mysql_stmt_bind_param(stmt, params);
        mysql_stmt_execute(stmt);
        
        int rowsCount = 0;
        while (mysql_stmt_fetch(stmt) == 0) rowsCount++;
        
        printf("ee : %d\n", rowsCount);
        if (rowsCount == 1) status = 1;
    }
    mysql_stmt_close(stmt);
    
    return status;
}

const char *getSaltByEmail(MYSQL *mysql, char *email) {
    char salt[7];
    MYSQL_STMT    *stmt;
    MYSQL_BIND    bind[1];
    MYSQL_RES     *prepare_meta_result;
    MYSQL_TIME    ts;
    unsigned long length[1];
    int           param_count, column_count, row_count;
    bool          is_null[1];
    bool          error[1];

/* Prepare a SELECT query to fetch data from test_table */
    stmt = mysql_stmt_init(mysql);
    if (!stmt)
    {
        fprintf(stderr, " mysql_stmt_init(), out of memory\n");
        return "ko";
    }
    if (mysql_stmt_prepare(stmt, "SELECT salt FROM users WHERE email = ?", strlen("SELECT salt FROM users WHERE email = ?")))
    {
        fprintf(stderr, " mysql_stmt_prepare(), SELECT failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        return "ko";
    }
    fprintf(stdout, " prepare, SELECT successful\n");
/* Get the parameter count from the statement */
    MYSQL_BIND params[1];
    memset(params, 0, sizeof(params));
    params[0].buffer_type = MYSQL_TYPE_VARCHAR;
    params[0].buffer = email;
    params[0].buffer_length = strlen(email);
    mysql_stmt_bind_param(stmt, params);
    param_count= mysql_stmt_param_count(stmt);
    fprintf(stdout, " total parameters in SELECT: %d\n", param_count);
    if (param_count != 1) /* validate parameter count */
    {
        fprintf(stderr, " invalid parameter count returned by MySQL\n");
        return "ko";
    }
/* Execute the SELECT query */
    if (mysql_stmt_execute(stmt))
    {
        fprintf(stderr, " mysql_stmt_execute(), failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        return "ko";
    }
/* Fetch result set meta information */
    prepare_meta_result = mysql_stmt_result_metadata(stmt);
    if (!prepare_meta_result)
    {
        fprintf(stderr,
                " mysql_stmt_result_metadata(), returned no meta information\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        return "ko";
    }
/* Get total columns in the query */
    column_count= mysql_num_fields(prepare_meta_result);
    fprintf(stdout,
            " total columns in SELECT statement: %d\n",
            column_count);

    if (column_count != 1) /* validate column count */
    {
        fprintf(stderr, " invalid column count returned by MySQL\n");
        return "ko";
    }
/* Bind the result buffers for all 4 columns before fetching them */
    memset(bind, 0, sizeof(bind));
/* STRING COLUMN */
    bind[0].buffer_type= MYSQL_TYPE_STRING;
    bind[0].buffer= (char *)salt;
    bind[0].buffer_length= strlen(salt);
    bind[0].is_null= &is_null[0];
    bind[0].length= &length[0];
    bind[0].error= &error[0];
/* Bind the result buffers */
    if (mysql_stmt_bind_result(stmt, bind))
    {
        fprintf(stderr, " mysql_stmt_bind_result() failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        return "ko";
    }
/* Now buffer all results to client (optional step) */
    if (mysql_stmt_store_result(stmt))
    {
        fprintf(stderr, " mysql_stmt_store_result() failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        return "ko";
    }
/* Fetch all rows */
    row_count= 0;
    fprintf(stdout, "Fetching results ...\n");
    mysql_stmt_fetch(stmt);
        row_count++;
        /* column 2 */
        fprintf(stdout, "   column2 (string)   : ");
        if (is_null[0])
            fprintf(stdout, " NULL\n");
        else
            fprintf(stdout, " %s(%ld)\n", salt, length[0]);

        fprintf(stdout, "\n");
/* Validate rows fetched */
    fprintf(stdout, " total rows fetched: %d\n", row_count);
    if (row_count != 1)
    {
        fprintf(stderr, " MySQL failed to return all rows\n");
        return "ko";
    }
/* Free the prepared result metadata */
    mysql_free_result(prepare_meta_result);
/* Close the statement */
    if (mysql_stmt_close(stmt))
    {
        fprintf(stderr, " failed while closing the statement\n");
        fprintf(stderr, " %s\n", mysql_error(mysql));
    }
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

        mysql_stmt_bind_param(stmt, params);
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

        mysql_stmt_bind_param(stmt, params);
        int status = mysql_stmt_execute(stmt);

        if (status == EXIT_SUCCESS) {
            int tmpId = 0;
            MYSQL_BIND results[1];
            memset(results, 0, sizeof(results));
            results[0].buffer_type = MYSQL_TYPE_LONG;
            results[0].buffer = &tmpId;
            mysql_stmt_bind_result(stmt, results);

            if (mysql_stmt_fetch(stmt) == 0) {
                userId = (tmpId > 0) ? tmpId : 0;
            }
        }
    }
    mysql_stmt_close(stmt);

    return userId;
}