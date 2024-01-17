#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "../../includes/webService.h"

size_t writeBodyToString(void *ptr, size_t size, size_t nmemb, void *stream) {
    size_t realSize = size * nmemb;
    BodyStruct *tmp = (BodyStruct *)stream;

    char *tmpPtr = (char *) realloc(tmp->memory, tmp->size + realSize + 1);
    if (tmpPtr == NULL) return 0;

    FILE *fp = fopen("./json/getLeaks.json", "w");
    if (fp != NULL) {
        fwrite(ptr, size, nmemb, fp);
        fclose(fp);
    }

    tmp->size += realSize;
    tmp->memory = tmpPtr;
    strncpy(tmp->memory, (char *) ptr, realSize);
    tmp->memory[tmp->size] = '\0';

    return realSize;
}

size_t writeBodyToJson(void *content, size_t size, size_t nmemb, void *stream) {
    size_t realSize = size * nmemb;
    JsonMemoryStruct *tmp = (JsonMemoryStruct *)stream;

    cJSON *root = cJSON_ParseWithLength((char *)content, realSize);
    if (root == NULL) return 0;

    FILE *fp = fopen("./json/getLeaks.json", "w");
    if (fp != NULL) {
        char *formattedJson = cJSON_Print(root);
        fprintf(fp, "%s", formattedJson);
        fclose(fp);
        free(formattedJson);
    }

    tmp->size += realSize;
    tmp->json = root;

    return realSize;
}

char *getHttpRequest(char *url) {
    const char *pathToCert = "./config/curl-ca.crt";
    CURL *curl;
    CURLcode res;

    BodyStruct chunkBody;
    chunkBody.size = 0;
    chunkBody.memory = (char *) malloc(1);
    
    curl = curl_easy_init();
    
    if(curl && url) {
        if (strstr(url, "http://") == NULL && strstr(url, "https://") == NULL) {
            printf("URL incorrecte!\n");
            return NULL;
        } else {
            curl_off_t downloadSize;

            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
            curl_easy_setopt(curl, CURLOPT_CAINFO, pathToCert);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeBodyToString);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunkBody);

            res = curl_easy_perform(curl);  // Code de retour
            if(res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            } else {
                res = curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &downloadSize);
                printf("dl size : %" CURL_FORMAT_CURL_OFF_T "\n", downloadSize);
                printf("llll :: %s\n", chunkBody.memory);
                printf("%lu bytes retrieved\n", (unsigned long)chunkBody.size);
                return chunkBody.memory;
            }
            
            curl_easy_cleanup(curl);    // clean de l'instance
        }
    }

    return NULL;
}

cJSON *getJsonFromRequest(char *url) {
    const char *pathToCert = CERT_CA_PATH;
    CURL *curl;
    CURLcode res;
    cJSON *jsonData = cJSON_CreateObject();
    
    curl = curl_easy_init();
    
    if(curl && url) {
        if (strstr(url, "http://") == NULL && strstr(url, "https://") == NULL) {
            return NULL;
        } else {
            curl_off_t downloadSize;

            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
            curl_easy_setopt(curl, CURLOPT_CAINFO, pathToCert);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeBodyToJson);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, jsonData);

            res = curl_easy_perform(curl);  // Code de retour
            if(res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            
            curl_easy_cleanup(curl);    // clean de l'instance
        }
    }

    return jsonData;
}

cJSON *getJsonFromGetRequest(char *url, char *key) {
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();

    JsonMemoryStruct chunkBody;
    chunkBody.size = 0;
    chunkBody.json = cJSON_CreateObject();
    
    if(curl && url) {
        if (strstr(url, "http://") == NULL && strstr(url, "https://") == NULL) {
            return NULL;
        } else {
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, "Accept: */*");
            headers = curl_slist_append(headers, "Accept-Encoding: gzip, deflate, br");
            headers = curl_slist_append(headers, "Connection: keep-alive");

            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_ENCODING, "");
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:121.0) Gecko/20100101 Firefox/121.");
            //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
            //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
            curl_easy_setopt(curl, CURLOPT_CAINFO, CERT_CA_PATH);
            //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
            //curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeBodyToJson);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunkBody);

            res = curl_easy_perform(curl);  // Code de retour
            if(res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);    // clean de l'instance
            return chunkBody.json;
        }
    }

    return NULL;
}