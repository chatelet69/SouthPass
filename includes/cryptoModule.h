#ifndef CRYPTOGRAPHY_MODULE_H
#define CRYPTOGRAPHY_MODULE_H

void handleErrors(void);
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,unsigned char *iv, unsigned char *plaintext);
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,unsigned char *iv, unsigned char *ciphertext);
unsigned char *encryptString(const char *plain, char *test);

#endif