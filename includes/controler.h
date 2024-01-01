#ifndef _CONTROLER_HEAD_FILE
#define _CONTROLER_HEAD_FILE

char **getPwsdList();
void printCreds(Credentials *creds, unsigned int size);
void freeCredsArray(CredsArray credsArray);
void printPCred(Credentials *creds);
void freeCredentialsData(Credentials *creds);

#endif