/*
    Filename : loginController.c
    Description : Login Controller (SignIn, Login, TokenFile...)
    Created by mathf on 03/01/2024
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <openssl/sha.h>
#include "../../includes/db.h"
#include "../../includes/backController.h"
#include "../../includes/backLoginSignIn.h"

int setConnected(){
    FILE * fp = fopen("../connectionLog.txt", "wb");
    fputs("connected", fp);
    fclose(fp);
    return 0;
}

int isConnected(){
    FILE * fp = fopen("../connectionLog.txt", "rb");
    int close;
    if(fp == NULL){
        FILE * fp2 = fopen("../connectionLog.txt", "wb");
        fputs("disconnected", fp2);
        close = fclose(fp2);
        return -1;
    }
    char isConnected[13];
    fgets(isConnected, 13, fp);
    close = fclose(fp);
    if(close != 0)
        return -1;
    if(strcmp(isConnected, "connected") == 0){
        return 0;
    }else if(strcmp(isConnected, "disconnected") == 0){
        return -1;
    }else{
        FILE * fp3 = fopen("../connectionLog.txt", "wb");
        fputs("disconnected", fp3);
        fclose(fp3);
        return -1;
    }
}

const char *verifSignIn(char *email, char *pwd, char *verifPwd, char *masterPwd, char *verifMasterPwd){
    char *pos;
    
    // verif email
    if(strlen(email) == 0)
        return "Email manquant";
    pos = strchr(email, '@');
    if(pos == NULL)
        return "Mauvais email";
    if(strchr(pos+1, '.')==NULL)
        return "Mauvais email";

    // verif pwd
    if(strlen(pwd)<10)
        return "Un mot de passe doit être suppérieur 10 caractères";
    if(strcmp(pwd, verifPwd) != 0)
        return "Mauvaise confirmation du mot de passe";
    if(verifPasswordChars(pwd) == 0)
        return "Vos mots de passes doivent contenir au moins 1 lettre, 1 chiffre et 1 caractère spécial";

    // verif pwd maitres
    if(strlen(masterPwd)<10)
        return "Un mot de passe doit être suppérieur 10 caractères";
    if(strcmp(masterPwd, verifMasterPwd) != 0)
        return "Mauvaise confirmation du mot de passe";
    if(strcmp(pwd, masterPwd) == 0)
        return "Vos mot de passe et mot de passe maitres doivent êtres différents";

    if(verifPasswordChars(masterPwd) == 0)
        return "Vos mots de passes doivent contenir au moins 1 lettre, 1 chiffre et 1 caractère spécial";

    return "ok";
}

int verifLogin(MYSQL *dbCon, char *email, char *password, char *masterPwd) {
    char salt[7];
    strcpy(salt, getSaltByEmail(dbCon, email));
    if(strcmp(salt, "ko") == 0){
        printf("KO");
        return 1;
    }
    
    char hashedPwd[257];
    char* hashString = (char*)malloc(2*SHA256_DIGEST_LENGTH+1);
    strcpy(hashedPwd, shaPwd(password, hashString, salt));
    free(hashString);

    char hashedMasterPwd[257];
    char* hashMasterString = (char*)malloc(2*SHA256_DIGEST_LENGTH+1);
    strcpy(hashedMasterPwd, shaPwd(masterPwd, hashMasterString, salt));
    free(hashMasterString);

    int status = checkLoginDb(dbCon, salt, email, hashedPwd, hashedMasterPwd);
    printf("STATUS = %d\n", status);
    if (status == 0) generateNewUserToken(dbCon, email);

    return (status == 0) ? EXIT_SUCCESS : status;
}

int verifPasswordChars(char * str){
    if(hasLetter(str) == 0)
        return 0;
    if(hasDigit(str) == 0)
        return 0;
    if(hasSpecialChar(str) == 0)
        return 0;

    return 1;
}

int hasLetter(char *str) {
    while (*str) {
        if (isalpha(*str)) {
            return 1;
        }
        str++;
    }
    return 0;
}

int hasDigit(char *str) {
    while (*str) {
        if (isdigit(*str)) {
            return 1;
        }
        str++;
    }
    return 0;
}

int hasSpecialChar(char *str) {
    while (*str) {
        if (!isalnum(*str)) {
            return 1;
        }
        str++;
    }
    return 0;
}