#ifndef FILE_CONTROLLER_H
#define FILE_CONTROLLER_H

int getThemePreference();
TokenInfos *getTokenFileInfos();
void saveThemePreference(int theme);

#define TOKEN_SIZE 65
#define MAIL_SIZE 50
#define CONFIG_FILE_PATH "config/config.ini"
#define COOKIE_FILE_PATH "cookie/cookie.bin"
#define CONFIG_FAILURE -1

#endif