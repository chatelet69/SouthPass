#include <string.h>
#include <curl/curl.h>
#include "../../includes/web_service.h"

int getHttpRequest(char *url) {
    const char *pathToCert = "./../config/curl-ca.crt";
    CURL *curl;
    CURLcode res;
    
    curl = curl_easy_init();
    
    if(curl && url) {
        if (strstr(url, "http://") == NULL && strstr(url, "https://") == NULL) {
            printf("URL incorrecte!\n");
        } else {
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
            curl_easy_setopt(curl, CURLOPT_CAINFO, pathToCert);
            // url is redirected, so we tell libcurl to follow redirection 
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

            // Perform the request, res will get the return code 
            res = curl_easy_perform(curl);
            // Check for errors 
            if(res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));

            // always cleanup 
            curl_easy_cleanup(curl);
        }
    }

    return 0;
}