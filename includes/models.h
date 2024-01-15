#ifndef MODELS_HEAD_FILE
#define MODELS_HEAD_FILE

union PasswordType {
    char *plainPassword;
    unsigned char *cipherPassword;
};

typedef struct Credentials Credentials;
struct Credentials {
    int id;
    int userId;
    char *name;
    char *loginName;
    char *password;
};

typedef struct TokenInfos {
    int id;
    char *token;
    char *email;
} TokenInfos;

typedef struct CredsArray {
    Credentials *credentials;
    unsigned int size;
} CredsArray;

typedef struct ExportList {
    char **lines;
    unsigned int count;
} ExportList;

typedef struct CredentialLeak {
    char *website;
    char *login;
    char *password;
    char *leakDate;
} CredentialLeak;

typedef struct LeaksList {
    struct CredentialLeak *credentialLeaks;
    int count;
} LeaksList;

typedef struct LoginsList {
    char **logins;
    int count;
} LoginsList;

#endif