//
// Created by mathf on 09/01/2024.
//
#include "../../includes/backPwdQuality.h"
#include "../../includes/pincludes.h"
#include <stdlib.h>
#include <stdio.h>
#include "../../includes/backLoginSignIn.h"

char * testPwd(char * pwd){
    int count;
    if(checkRockYou(pwd) == 1)
        return "Faible";
    if(strlen(pwd)<8)
        return "Faible";
    if(hasSpecialChar(pwd))
        count++;
    if(hasDigit(pwd))
        count++;
    if(hasLetter(pwd))
        count++;
    if(count <= 2)
        return "Moyen";
    if(strlen(pwd)<16)
        return "Moyen";
    return "Fort";
}

int checkRockYou(char * pwd){
    FILE * fp = fopen("../../rockyou.txt", "rt");
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
    }
    fclose(fp);
    return 0;
}