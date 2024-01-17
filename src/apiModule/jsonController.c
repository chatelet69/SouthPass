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

CredentialLeak *parseJsonToLeaksList(struct LeaksList *list, cJSON *json, char *login) {
    const cJSON *successStatusItem = cJSON_GetObjectItem(json, "success");
    if (cJSON_IsBool(successStatusItem) && successStatusItem->valueint == 1) {
        int index = list->count;
        cJSON *sources = cJSON_GetObjectItem(json, "sources");
        cJSON *foundCount = cJSON_GetObjectItem(json, "found");
        cJSON *source = NULL;

        // Si une liste de credentials existe on réalloue un nouveau bloc avec realloc
        if (list->credentialLeaks == NULL) {
            list->credentialLeaks = (CredentialLeak *) malloc(sizeof(CredentialLeak) * foundCount->valueint);
        } else {
            list->credentialLeaks = (CredentialLeak *) realloc(list->credentialLeaks, sizeof(CredentialLeak) * (foundCount->valueint + list->count));
        }

        if (list->credentialLeaks != NULL) {
            cJSON_ArrayForEach(source, sources) {
                cJSON *name = cJSON_GetObjectItem(source, "name");
                cJSON *date = cJSON_GetObjectItem(source, "date");
                if (name != NULL) {
                    list->credentialLeaks[index].website = strdup(cJSON_Print(name));
                    list->credentialLeaks[index].login = strdup(login);
                    list->credentialLeaks[index].leakDate = strdup(cJSON_Print(date));
                    list->count += 1;
                    index++;
                }
            }
        }
    }

    // On retourne la nouvelle adresse de la liste de credentialLeaks
    return list->credentialLeaks;
}