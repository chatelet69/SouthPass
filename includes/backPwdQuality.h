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
    char ** pwd;
    unsigned int size;
};
struct Website{
    char * website;
    char * username;
};

struct WebsiteByPwd{
    struct Website * website;
    unsigned int size;
};
// WeakPwdList
struct WeakPwdList * getAllWeaksPwd(struct WeakPwdList *start, MYSQL * dbCon);
struct WeakPwdList * addWeakPwd(struct WeakPwdList *start, char * site, char * username, char * pwd);
void printWeaksPwd(struct WeakPwdList *start);

// PwdQuality
const char *testPwd(char * pwd);
int checkRockYou(char * pwd);

#endif //SOUTHPASS_BACKPWDQUALITY_H
