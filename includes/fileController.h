#ifndef FILE_CONTROLLER_H
#define FILE_CONTROLLER_H

#ifdef _WIN32
#define IS_WINDOWS 1
#define IS_LINUX 0
#else
#define IS_WINDOWS 0
#define IS_LINUX 1
#endif

int getThemePreference();
TokenInfos *getTokenFileInfos();
void saveThemePreference(int theme);
int saveNewTokenFile(char *tokenHash, char *email, const int id);

#define TOKEN_SIZE 65
#define MAIL_SIZE 50
#define CONFIG_FILE_PATH "config/config.ini"
#define COOKIE_FILE_PATH "cookie/cookie.bin"
#define CONFIG_FAILURE -1

#endif