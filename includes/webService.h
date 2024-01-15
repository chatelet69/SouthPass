#ifndef WEB_SERVICE_H
#define WEB_SERVICE_H

#include <cjson/cJSON.h>

#define CART_PATH "./config/curl-ca.crt"

typedef struct BodyStruct {
  char *memory;
  size_t size;
} BodyStruct;

size_t writeBodyToString(void *ptr, size_t size, size_t nmemb, void *stream);
size_t writeBodyToJson(void *content, size_t size, size_t nmemb, void *stream);
char *getHttpRequest(char *url);
cJSON *getJsonFromRequest(char *url);

#endif