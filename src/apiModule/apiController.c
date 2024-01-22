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

LeaksList *getDataLeaksFromLeakCheck(MYSQL *dbCon, const int userId) {
    if (userId == 0) return NULL;
    char *key = getLeakCheckKey();  // Récupération de la clé dans le fichier de config
    if (key == NULL) key = strdup(LEAKCHECK_DEFAULT_KEY);   // Si pas de clé utilisation de celle par défaut
    LeaksList *leaksList = (LeaksList *) malloc(sizeof(LeaksList));
    leaksList->count = 0;
    leaksList->credentialLeaks = NULL;

    LoginsList *uniquesLogins = getUniquesLoginsById(dbCon, userId);    // Récupération des emails uniques
    if (uniquesLogins != NULL && key != NULL) {
        for (int i = 0; i < uniquesLogins->count; i++) {
            char *actualLogin = strdup(uniquesLogins->logins[i]);
            char *url = (char *) malloc(sizeof(char) * (strlen(LEAKCHECK_BASE_URL) + strlen(actualLogin) + strlen(key) + 15));
            sprintf(url, "%s?key=%s&check=%s", LEAKCHECK_BASE_URL, key, actualLogin);
            // Ecriture de l'URL finale pour la requête d'API
            
            cJSON *resData = getJsonFromGetRequest(url, strdup(key));
            leaksList->credentialLeaks = parseJsonToLeaksList(leaksList, resData, actualLogin);
            free(url);
            free(actualLogin);
        }
    }
    
    // Clean
    free(uniquesLogins);
    if (key != NULL) free(key);

    return leaksList;
}