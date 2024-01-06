/*
    Filename : fileController.c
    Description : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../includes/models.h"
#include "../../includes/utils.h"
#include "../../includes/fileController.h"

int getThemePreference() {
    FILE *configFile = fopen(CONFIG_FILE_PATH, "rt");
    if (configFile == NULL) return CONFIG_FAILURE;

    char line[121];
    char prop[2][40];
    while (fgets(line, 120, configFile) != NULL) {
        int res = sscanf(line, "%s : %s", prop[0], prop[1]);
        if (res == 2) {
            if (strcmp(prop[0], "theme_preference") == 0) {
                return (strcmp(prop[1], "dark") == 0 ? 1 : 0);
            }
        }
    }

    fclose(configFile);

    return 0;
}

TokenInfos *getTokenFileInfos() {
    FILE *cookieFile = fopen(COOKIE_FILE_PATH, "rb");
    if (cookieFile == NULL) return NULL;

    TokenInfos *tokenInfos = NULL;
    char line[130];
    char token[TOKEN_SIZE], email[MAIL_SIZE];
    int id = 0;

    int res = fscanf(cookieFile, "%[^:]:%[^:]:%d", token, email, &id);
    if (res == 3) {
        tokenInfos = (TokenInfos *) malloc(sizeof(TokenInfos));
        tokenInfos->id = id;
        tokenInfos->email = strdup(email);
        tokenInfos->token = strdup(token);
    };

    fclose(cookieFile);

    return tokenInfos;
}

void saveThemePreference(int theme) {
    FILE *configFile = fopen(CONFIG_FILE_PATH, "rt+");
    const char *savedTheme = (theme) ? "dark" : "light";

    if (configFile != NULL) {
        long pos = 0;
        char line[121];
        char prop[2][40];
        while (fgets(line, 120, configFile) != NULL) {
            if (strstr(line, "theme_preference") != NULL) {
                pos = ftell(configFile) - strlen(line);
                break;
            }
        }
        fseek(configFile, pos, SEEK_SET);
        fprintf(configFile, "theme_preference : %s\n", savedTheme);
    }

    fclose(configFile);
}