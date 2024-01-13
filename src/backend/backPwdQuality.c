//
// Created by mathf on 09/01/2024.
//
#include "../../includes/backPwdQuality.h"
#include "../../includes/pincludes.h"
#include <stdlib.h>
#include <stdio.h>
#include "../../includes/fileController.h"
#include "../../includes/backLoginSignIn.h"

char * testPwd(char * pwd){
     int count;
    if(hasSpecialChar(pwd))
        count++;
    if(hasDigit(pwd))
        count++;
    if(hasLetter(pwd))
        count++;

    if(checkRockYou(pwd) == 1 || strlen(pwd)<8 || count == 1)
        return "Faible";
    if(count == 2 || strlen(pwd)<14)
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
    CredsArray credsArray = getPasswordsList(dbCon, tokenInfos->id);
    for (int i = 0; i < credsArray.size; ++i) {
        if(strcmp(testPwd(credsArray.creds[i].password), "Faible") == 0)
            start = addWeakPwd(start, credsArray.creds[i].name, credsArray.creds[i].loginName, credsArray.creds[i].password);
    }
    freeCredsArray(credsArray);
    printWeaksPwd(start);
    return start;
}

struct WeakPwdList * addWeakPwd(struct WeakPwdList *start, char * url, char * username, char * pwd){
    struct WeakPwdList *inter = (struct WeakPwdList*)malloc(sizeof(struct WeakPwdList));
    inter->site = url;
    inter->username = username;
    inter->pwd = pwd;
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