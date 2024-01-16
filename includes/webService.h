#ifndef WEB_SERVICE_H
#define WEB_SERVICE_H

#include <cjson/cJSON.h>

#define CERT_CA_PATH "./config/curl-ca.crt"

typedef struct BodyStruct {
  char *memory;
  size_t size;
} BodyStruct;

typedef struct JsonMemoryStruct {
  cJSON *json;
  size_t size;
} JsonMemoryStruct;

size_t writeBodyToString(void *ptr, size_t size, size_t nmemb, void *stream);
size_t writeBodyToJson(void *content, size_t size, size_t nmemb, void *stream);
char *getHttpRequest(char *url);
cJSON *getJsonFromRequest(char *url);
cJSON *getJsonFromGetRequest(char *url, char *key);

#endif