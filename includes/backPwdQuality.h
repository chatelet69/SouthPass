//
// Created by mathf on 09/01/2024.
//

#ifndef SOUTHPASS_BACKPWDQUALITY_H
#define SOUTHPASS_BACKPWDQUALITY_H

#include <mysql.h>

struct WeakPwdList{
    char * site;
    char * username;
    char * pwd;
    struct WeakPwdList *next;
};
/*
struct Websites{
    char * url;
    char * username;
    struct Websites *next;
};

struct ReUsedPwdByWebsite{
    char * pwd;
    struct Websites * websites;
    struct ReUsedPwdByWebsite *next;
};
*/

// WeakPwdList
struct WeakPwdList * getAllWeaksPwd(struct WeakPwdList *start, MYSQL * dbCon);
struct WeakPwdList * addWeakPwd(struct WeakPwdList *start, char * site, char * username, char * pwd);
void printWeaksPwd(struct WeakPwdList *start);

// ReUsedPwd
/*
struct ReUsedPwdByWebsite * getReUsedPwd(struct ReUsedPwdByWebsite * websiteByPwdArray, MYSQL * dbCon);
struct Websites * addWebsitesByPwd(struct Websites *prevWeb, char * url, char * username);
void printReUsedPwd(struct ReUsedPwdByWebsite *websiteByPwdArray);
struct ReUsedPwdByWebsite * addPwd(struct ReUsedPwdByWebsite * websiteByPwdArray,char * pwd, char * url, char * username);
int verifIfPwdExist(struct ReUsedPwdByWebsite *websiteByPwdArray, char * pwd);
void freeWebsitesList(struct Websites *webList);
*/



// PwdQuality
char * testPwd(char * pwd);
int checkRockYou(char * pwd);
#endif //SOUTHPASS_BACKPWDQUALITY_H
