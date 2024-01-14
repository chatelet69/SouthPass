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
    inter->site = strdup(url);
    inter->username = strdup(username);
    inter->pwd = strdup(pwd);
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

// ReUsed PWD
/*
struct ReUsedPwdByWebsite * getReUsedPwd(struct ReUsedPwdByWebsite * websiteByPwdArray, MYSQL * dbCon){
    TokenInfos *tokenInfos = getTokenFileInfos();
    CredsArray credsArray = getPasswordsList(dbCon, tokenInfos->id);

    for (int i = 0; i < credsArray.size; ++i) {
        int check = 0;
        for (int j = i+1; j < credsArray.size; ++j) {
            if(strcmp(credsArray.creds[i].password, credsArray.creds[j].password) == 0){
                if(check == 0 && verifIfPwdExist(websiteByPwdArray, credsArray.creds[i].password) == 0){
                    check = 1;
                    websiteByPwdArray = addPwd(websiteByPwdArray, credsArray.creds[i].password, credsArray.creds[i].name, credsArray.creds[i].loginName);
                }
                if(websiteByPwdArray!=NULL && websiteByPwdArray->websites!=NULL){
                    websiteByPwdArray->websites = addWebsitesByPwd(websiteByPwdArray->websites,credsArray.creds[j].name, credsArray.creds[j].loginName);
                }
            }
        }
        if(websiteByPwdArray!=NULL && websiteByPwdArray->next != NULL)
            websiteByPwdArray = websiteByPwdArray->next;
    }
    freeCredsArray(credsArray);
    return websiteByPwdArray;
}

struct ReUsedPwdByWebsite * addPwd(struct ReUsedPwdByWebsite * websiteByPwdArray,char * pwd, char * url, char * username){
    struct ReUsedPwdByWebsite *inter = (struct ReUsedPwdByWebsite*)malloc(sizeof(struct ReUsedPwdByWebsite));
    printf("\n\n        PWD CATEGORY CREATED : %s", pwd);

    inter->pwd = strdup(pwd);
    inter->websites = addWebsitesByPwd(inter->websites, url, username);
    inter->next = websiteByPwdArray;

    return inter;
}

struct Websites * addWebsitesByPwd(struct Websites *prevWeb, char * url, char * username){
    struct Websites *inter = (struct Websites*)malloc(sizeof(struct Websites));
    inter->url = strdup(url);
    inter->username = strdup(username);
    inter->next = prevWeb;

    printf("\nWEBSITE ADDED : url : %s, username : %s next : %p", inter->url, inter->username, inter->next);

    return inter;
}


void printReUsedPwd(struct ReUsedPwdByWebsite *websiteByPwdArray){
    struct ReUsedPwdByWebsite *interPwd = websiteByPwdArray;
    if(interPwd == NULL)
        printf("\nAucun mot de passe ne se repetent");

    while(interPwd != NULL){
        printf("\n\nMots de passes qui se repetent pour le mot de passe : %s", interPwd->pwd);
        while(interPwd->websites != NULL){
            if(interPwd->websites->url != NULL && interPwd->websites->username != NULL)
                printf("\nurl : %s, username : %s", interPwd->websites->url, interPwd->websites->username);

            interPwd->websites = interPwd->websites->next;
        }
        interPwd = interPwd->next;
    }
}

int verifIfPwdExist(struct ReUsedPwdByWebsite *websiteByPwdArray, char * pwd){
    struct ReUsedPwdByWebsite *interPwd = websiteByPwdArray;
    if(interPwd == NULL)
        return 0;

    while(interPwd!=NULL){
        if(strcmp(interPwd->pwd, pwd) == 0)
            return 1;
        interPwd = interPwd ->next;
    }
    return 0;
}
*/


