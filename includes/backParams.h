//
// Created by mathf on 19/01/2024.
//

#ifndef SOUTHPASS_BACKPARAMS_H
#define SOUTHPASS_BACKPARAMS_H
#include <stdlib.h>
#include <stdio.h>
#include "./db.h"
#include "./backLoginSignIn.h"
int verifPwd(MYSQL *dbCon, char * pwd, char * verifPwd, char *email);

#endif //SOUTHPASS_BACKPARAMS_H
