/*
    Filename : jsonController.c
    Description : Json Functions to encode and parse json data
*/

#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include "../../includes/jsonController.h"

void printJson(cJSON *item) {
    if (item != NULL) {
        char *jsonString = cJSON_Print(item);
        printf("%s", jsonString);
        free(jsonString);
    }
}

void parseJsonFromString(char *str) {
    
}