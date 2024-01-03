//
// Created by mathf on 03/01/2024.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int isConnected(){
    FILE * fp = fopen("../connectionLog.txt", "rb");
    int close;
    if(fp == NULL){
        FILE * fp2 = fopen("../connectionLog.txt", "wb");
        fputs("disconnected", fp2);
        close = fclose(fp2);
        return -1;
    }
    char isConnected[13];
    fgets(isConnected, 13, fp);
    close = fclose(fp);
    if(close != 0)
        return -1;
    if(strcmp(isConnected, "connected") == 0){
        return 0;
    }else if(strcmp(isConnected, "disconnected") == 0){
        return -1;
    }else{
        FILE * fp3 = fopen("../connectionLog.txt", "wb");
        fputs("disconnected", fp3);
        fclose(fp3);
        return -1;
    }
}