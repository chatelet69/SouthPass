#ifndef MODELS_HEAD_FILE
#define MODELS_HEAD_FILE

typedef struct Credentials Credentials;
struct Credentials {
    int id;
    int userId;
    char *name;
    char *loginName;
    char *password;
};

typedef struct {
    int id;
    char *token;
    char *email;
} TokenInfos;

typedef struct {
    Credentials *creds;
    unsigned int size;
} CredsArray;

#endif