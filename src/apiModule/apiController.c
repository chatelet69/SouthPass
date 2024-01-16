/*
    Filename : apiController.c
    Description : Functions that interact with IntelX / LeakCheck Api, for the leak alerts check
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../includes/db.h"
#include "../../includes/models.h"
#include "../../includes/backController.h"
#include "../../includes/webService.h"
#include "../../includes/fileController.h"
#include "../../includes/jsonController.h"

void printLeaksList(struct LeaksList *list) {
    printf("---- > Leaks List : \n");
    if (list != NULL) {
        for (unsigned int i = 0; i < list->count; i++) {
            printf("Website : %s\nLogin : %s\nDate : %s\n", list->credentialLeaks[i].website, 
                                                            list->credentialLeaks[i].login,
                                                            list->credentialLeaks[i].leakDate);
            printf("------------\n");
        }
    }
}

void freeLeaksList(struct LeaksList *list) {
    if (list != NULL) {
        for (unsigned int i = 0; i < list->count; i++) {
            free(list->credentialLeaks[i].login);
            free(list->credentialLeaks[i].website);
            free(list->credentialLeaks[i].leakDate);
        }
        free(list->credentialLeaks);
        list->count = 0;
    }
}

LeaksList *getDataLeaks(MYSQL *dbCon, const int userId) {
    if (userId == 0) return NULL;

    //LoginsList = getUniquesLoginsById(dbCon, userId);
    char *url = strdup("https://2.intelx.io/intelligent/search");
    cJSON *resData = getJsonFromRequest(url);
    free(url);
    printf("RES DATA : %s\n", cJSON_Print(resData));

    return NULL;
}

int getRemainingCreditsIntelx() {
    char *url = strdup("https://2.intelx.io/authenticate/info");
    char *intelxKey = strdup("replace by the function get intelx key");
    cJSON *resData = getJsonFromGetRequest(url, intelxKey);
    free(url);
    free(intelxKey);

    if (resData != NULL) {
        printf("%s", resData);
    }

    return 0;
}

LeaksList *getDataLeaksFromLeakCheck(MYSQL *dbCon, const int userId) {
    if (userId == 0) return NULL;
    char *key = getLeakCheckKey();
    printf("key : %s\n", key);

    LoginsList *uniquesLogins = getUniquesLoginsById(dbCon, userId);
    if (uniquesLogins != NULL) {
        char *actualLogin = NULL;
        for (int i = 0; i < uniquesLogins->count; i++) {
            printf("uniques : %s\n", uniquesLogins->logins[i]);
            actualLogin = strdup(uniquesLogins->logins[i]);
        }
        if (actualLogin != NULL) free(actualLogin);
    }
    char *login = strdup("helloyou@hotmail.com");
    char *url = (char *) malloc(sizeof(char) * (strlen(LEAKCHECK_BASE_URL) + strlen(login) + strlen(key) + 15));
    sprintf(url, "%s?key=%s&check=%s", LEAKCHECK_BASE_URL, key, login);
    
    cJSON *resData = getJsonFromGetRequest(url, strdup(key));
    free(url);
    free(uniquesLogins);
    printf("RES DATA : %s\n", cJSON_Print(resData));

    LeaksList *leaksList = parseJsonToLeaksList(resData, login);
    printLeaksList(leaksList);

    // Clean des pointeurs
    free(login);
    freeLeaksList(leaksList);
    free(leaksList);
    free(key);

    return NULL;
}