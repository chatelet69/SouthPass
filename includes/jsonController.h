#ifndef JSON_CONTROLLER_H
#define JSON_CONTROLLER_H

#include "models.h"

void printJson(cJSON *item);
CredentialLeak *parseJsonToLeaksList(struct LeaksList *list, cJSON *json, char *login);

#endif