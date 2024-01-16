/*
    Filename : jsonController.c
    Description : Json Functions to encode and parse json data
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include "../../includes/jsonController.h"

void printJson(cJSON *item) {
    if (item != NULL) {
        char *jsonString = cJSON_Print(item);
        printf("%s", jsonString);
        free(jsonString);
    }
}

LeaksList *parseJsonToLeaksList(cJSON *json, char *login) {
    LeaksList *list = (LeaksList *) malloc(sizeof(LeaksList));
    list->count = 0;
    list->credentialLeaks = NULL;

    const cJSON *successStatusItem = cJSON_GetObjectItem(json, "success");
    if (cJSON_IsBool(successStatusItem) && successStatusItem->valueint == 1) {
        int index = 0;
        cJSON *sources = cJSON_GetObjectItem(json, "sources");
        cJSON *foundCount = cJSON_GetObjectItem(json, "found");
        cJSON *source = NULL;

        list->credentialLeaks = (CredentialLeak *) malloc(sizeof(CredentialLeak) * foundCount->valueint);
        cJSON_ArrayForEach(source, sources) {
            cJSON *name = cJSON_GetObjectItem(source, "name");
            cJSON *date = cJSON_GetObjectItem(source, "date");
            if (name != NULL) {
                printf("name ==== %s\n", cJSON_Print(name));
                list->credentialLeaks[index].website = strdup(cJSON_Print(name));
                list->credentialLeaks[index].login = strdup(login);
                list->credentialLeaks[index].leakDate = strdup(cJSON_Print(date));
                list->count += 1;
                index++;
            }
        }
    }

    return list;
}