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
struct WeakPwdList * addItem(struct WeakPwdList *start, int value);

struct WeakPwdList * getAllWeaksPwd(struct WeakPwdList *start, MYSQL * dbCon);
struct WeakPwdList * addWeakPwd(struct WeakPwdList *start, char * site, char * username, char * pwd);
void printWeaksPwd(struct WeakPwdList *start);
char * testPwd(char * pwd);
int checkRockYou(char * pwd);
#endif //SOUTHPASS_BACKPWDQUALITY_H
