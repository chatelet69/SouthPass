/*
    Filename : backPwdQuality.c
    Description : File containing the functions managing the backend of the Password Quality page
    Last Edit : 21_01_2024
*/

#include <stdio.h>
#include <stdlib.h>
#include "../../includes/backPwdQuality.h"
#include "../../includes/pincludes.h"
#include <string.h>
#include "../../includes/fileController.h"
#include "../../includes/backLoginSignIn.h"

const char *testPwd(char * pwd){
    if(pwd == NULL)
        return NULL;

    int count = 0;
    if(hasSpecialChar(pwd))
        count++;
    if(hasDigit(pwd))
        count++;
    if(hasLetter(pwd))
        count++;

    if(checkRockYou(pwd) == 1 || strlen(pwd)<8 || count == 1)
        return "Faible";
    if(count >= 2 && strlen(pwd)<14)
        return "Moyen";

    return "Fort";
}

int checkRockYou(char * pwd){
    FILE * fp = fopen("../rockyou.txt", "rt");
    char verifPwd[50];
    if(fp != NULL){
        while (fgets(verifPwd, 50, fp) != NULL){
            if(verifPwd[strlen(verifPwd)-1] == '\n')
                verifPwd[strlen(verifPwd)-1] = '\0';

            if(strcmp(pwd, verifPwd) == 0){
                fclose(fp);
                return 1;
            }
        }
        fclose(fp);
    }
    return 0;
}

struct WeakPwdList * getAllWeaksPwd(struct WeakPwdList *start, MYSQL * dbCon){
    TokenInfos *tokenInfos = getTokenFileInfos();
    if(tokenInfos != NULL) {
        CredsArray *credsArray = getPasswordsList(dbCon, tokenInfos->id);
        for (int i = 0; i < credsArray->size; ++i) {
            if (strcmp(testPwd(credsArray->credentials[i].password), "Faible") == 0)
                start = addWeakPwd(start, credsArray->credentials[i].name, credsArray->credentials[i].loginName,credsArray->credentials[i].password, credsArray->credentials[i].id, credsArray->credentials[i].userId);
        }
        freeCredsArray(credsArray);
        // printWeaksPwd(start);
        return start;
    }else{
        return NULL;
    }
}

struct WeakPwdList * addWeakPwd(struct WeakPwdList *start, char * url, char * username, char * pwd, int id, int userId){
    struct WeakPwdList *inter = (struct WeakPwdList*)malloc(sizeof(struct WeakPwdList));
    inter->id = id;
    inter->userId = userId;
    inter->site = strdup(url);
    inter->username = strdup(username);
    inter->pwd = strdup(pwd);
    inter->next = start;

    return inter;
}

struct reUsedPwd * addWebsiteByPwd(struct reUsedPwd *start, struct Website *website){
    struct reUsedPwd *inter;
    inter = (struct reUsedPwd *)malloc(sizeof(struct reUsedPwd));
    inter->id = website->id;
    inter->userId = website->userId;
    inter->site = strdup(website->website);
    inter->username = strdup(website->username);
    inter->next = start;
    return inter;
}

void printWeaksPwd(struct WeakPwdList *start){
    struct WeakPwdList *inter = start;
    if(inter == NULL)
       printf("\nListe vide.");
    else
        printf("\nListe de pwd faible :");

    while(inter!=NULL){
        printf("\n%s", inter->pwd);
        inter = inter->next;
    }
}