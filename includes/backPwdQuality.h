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

struct PwdList{
    char * pwd;
    unsigned int size;
};

struct WebsiteByPwd{
    char * website;
    char * username;
    unsigned int size;
};

// WeakPwdList
struct WeakPwdList * getAllWeaksPwd(struct WeakPwdList *start, MYSQL * dbCon);
struct WeakPwdList * addWeakPwd(struct WeakPwdList *start, char * site, char * username, char * pwd);
void printWeaksPwd(struct WeakPwdList *start);

// PwdQuality
char * testPwd(char * pwd);
int checkRockYou(char * pwd);

// ReUsedPwd












// ReUsedPwd fail
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
/*
struct ReUsedPwdByWebsite * getReUsedPwd(struct ReUsedPwdByWebsite * websiteByPwdArray, MYSQL * dbCon);
struct Websites * addWebsitesByPwd(struct Websites *prevWeb, char * url, char * username);
void printReUsedPwd(struct ReUsedPwdByWebsite *websiteByPwdArray);
struct ReUsedPwdByWebsite * addPwd(struct ReUsedPwdByWebsite * websiteByPwdArray,char * pwd, char * url, char * username);
int verifIfPwdExist(struct ReUsedPwdByWebsite *websiteByPwdArray, char * pwd);
void freeWebsitesList(struct Websites *webList);
*/
#endif //SOUTHPASS_BACKPWDQUALITY_H
