//
// Created by mathf on 19/01/2024.
//

#include "../../includes/backParams.h"
#include <openssl/sha.h>

int verifPwd(MYSQL *dbCon, char * pwd, char * verifPwd, char * email){
    if(verifPasswordChars(pwd) == 0 || strlen(pwd)<10)
        return 1;
    if(verifPasswordChars(verifPwd) == 0 || strlen(verifPwd)<10)
        return 1;
    if(strcmp(pwd, verifPwd) != 0)
        return 2;

    int id = getUserIdBy(dbCon, email, "email");
    char salt[6];
    strcpy(salt, getSaltByEmail(dbCon, email));
    char* hashPwd = (char*)malloc(2*SHA256_DIGEST_LENGTH+1);
    char hashedPwd[65];
    strcpy(hashedPwd, shaPwd(pwd, hashPwd, salt));
    free(hashPwd);
    if(updatePwd(dbCon, hashedPwd, id, "Account"))
        return 3;

    return 0;
}
