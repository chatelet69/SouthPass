#ifndef CONTROLLER_H
#define CONTROLLER_H

char **getPwsdList();
void printCreds(Credentials *creds, unsigned int size);
void freeCredsArray(CredsArray credsArray);
void printPCred(Credentials *creds);
void freeCredentialsData(Credentials *creds);

#endif