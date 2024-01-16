#ifndef JSON_CONTROLLER_H
#define JSON_CONTROLLER_H

#include "models.h"

void printJson(cJSON *item);
LeaksList *parseJsonToLeaksList(cJSON *json, char *login);

#endif