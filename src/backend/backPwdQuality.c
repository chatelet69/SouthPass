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
    if(count <= 2 && strlen(pwd)<16)
        return "Moyen";

    return "Fort";
}

int checkRockYou(char * pwd){
    printf("PWD : %s", pwd);
    FILE * fp = fopen("C:\\Users\\mathf\\OneDrive\\Bureau\\Cours\\S1\\Langage_C_avance\\Southpass\\SouthPass\\rockyou.txt", "rt");
    char verifPwd[50];
    if(fp != NULL){
        while (fgets(verifPwd, 50, fp)!=NULL){
            if(strcmp(pwd, verifPwd) == 0){
                fclose(fp);
                printf("\nfound");
                return 1;
            }
        }
    }
    fclose(fp);
    printf("\nnot found");
    return 0;
}