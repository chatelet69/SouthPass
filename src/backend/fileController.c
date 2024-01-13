/*
    Filename : fileController.c
    Description : All functions that have action on files (token and config file)
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

    if (cookieFile != NULL) {
        int res = fscanf(cookieFile, "%[^:]:%[^:]:%d", token, email, &id);
        if (res == 3) {
            tokenInfos = (TokenInfos *) malloc(sizeof(TokenInfos));
            tokenInfos->id = id;
            tokenInfos->email = strdup(email);
            tokenInfos->token = strdup(token);
        };
        fclose(cookieFile);
    }

    return tokenInfos;
}

const int getUserIdByCookieFile() {
    FILE *cookieFile = fopen(COOKIE_FILE_PATH, "rb");
    if (cookieFile == NULL) return 0;

    int userId = 0;
    if (cookieFile != NULL) {
        int res = fscanf(cookieFile, "%*[^:]:%*[^:]:%d", &userId);
        fclose(cookieFile);
    }

    return userId;
}

int saveNewTokenFile(char *tokenHash, char *email, const int id) {
    FILE *cookieFile = fopen(COOKIE_FILE_PATH, "wb");
    if (cookieFile == NULL) return EXIT_FAILURE;

    int tokenSize = strlen(tokenHash);
    int emailSize = strlen(email);
    char *finalStr = (char *) malloc(sizeof(char) * (emailSize + tokenSize) + 8);
    sprintf(finalStr, "%s:%s:%d", tokenHash, email, id);

    fwrite(finalStr, sizeof(char), strlen(finalStr), cookieFile);
    fclose(cookieFile);

    return EXIT_SUCCESS;
}

void saveThemePreference(int theme) {
    FILE *configFile = fopen(CONFIG_FILE_PATH, "r+t");
    const char *savedTheme = (theme) ? "dark\0" : "light\0";

    if (configFile != NULL) {
        long pos = 0;
        char line[100];
        while (fgets(line, 100, configFile) != NULL) {
            pos = ftell(configFile) - strlen(line);
            printf("%ld : %s", pos, line);
            if (strstr(line, "theme_preference") != NULL) {
                printf("%ld : %ld\n", ftell(configFile), strlen(line));
                break;
            }
        }
        fseek(configFile, pos-1, SEEK_SET);
        if (IS_WINDOWS) fprintf(configFile, "theme_preference : %s\n", (theme) ? "dark" : "light");
        else if (IS_LINUX) fprintf(configFile, "theme_preference : %s\n", savedTheme);
        fclose(configFile);
    }

    fclose(configFile);
}

int createTokenFile() {
    FILE *cookieFile = fopen(COOKIE_FILE_PATH, "wb");
    if (cookieFile == NULL) return 1;
    fclose(cookieFile);

    return EXIT_SUCCESS;
}

int writePasswordsExportFile(char **passwordsFormatedList, int size, char *exportFolder) {
    char *userDownloadsPath = (char *)malloc(sizeof(char) * strlen(exportFolder) + 30);
    sprintf(userDownloadsPath, "%s/exportPasswords.csv", exportFolder);
    FILE *exportFile = fopen(userDownloadsPath, "wt");

    if (exportFile != NULL) {
        fprintf(exportFile, "Name,Login,Password\n");
        for (int i = 0; i < size; i++) fprintf(exportFile, "%s\n", passwordsFormatedList[i]);
        fclose(exportFile);
        return EXIT_SUCCESS;
    }
    free(userDownloadsPath);
    
    return EXIT_FAILURE;
}

char *getUserDirectoryPath(const char *osName) {
    if (strcmp(osName, "linux") == 0) {
        char *homeDir = getenv("HOME");
        if (homeDir != NULL) {
            return homeDir;
        } else {
            return strdup("/");
        }
    } /*else if (strcmp(osName, "windows") == 0) {
        //const char path[81];
        //else return NULL;
    }*/
    return NULL;
}

const char *getOsName() {
    #ifdef _WIN32
        return "windows";
    #elif __linux__
        return "linux";
    #else
        return "unfound";
    #endif
}