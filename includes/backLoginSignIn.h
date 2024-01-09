//
// Created by mathf on 03/01/2024.
//

#ifndef SOUTHPASS_BACKISCONNECTED_H
#define SOUTHPASS_BACKISCONNECTED_H

int isConnected();
const char *verifSignIn(char *, char *, char *, char *, char *);
int verifLogin(MYSQL *dbCon, char *email, char *password, char *masterPwd);
int hasLetter(char *);
int hasDigit(char *);
int hasSpecialChar(char *);
int verifPasswordChars(char *);
int verifPasswordValidity(char *, char *);
int setConnected();
char *shaPwd(const char *, char *, char *);

#endif
