//
// Created by mathf on 08/01/2024.
//

#include "../../includes/pwdGenController.h"


char * generatePwd(int length, int useMajChars, int useNumbers, int useSymbols){
    printf("\nmaj : %d, num : %d, sym : %d : length : %d\n", useMajChars, useNumbers, useSymbols, length);

    FILE * fp = fopen("./prevpwd.txt", "wt");
    srand((time(NULL)));
    char pass[42];

    char capLetter;
    for (int i = 0; i < length; i++) {
        int random = rand()%4;

        if(useMajChars && random == 0){
            capLetter = 'A' + (rand() % 26);
        }else if(random == 0)
            capLetter = 'a' + (rand() % 26);

        if(useNumbers && random == 1){
            capLetter = '0' + (rand() % 9);
        }else if(random == 1)
            capLetter = 'a' + (rand() % 26);

        if(useSymbols && random == 2){
            if(rand()%2==1)
                capLetter = '!' + (rand() % 15);
            else
                capLetter = ':' + (rand() % 7);
        }else if(random == 2)
            capLetter = 'a' + (rand() % 26);

        if(random == 3)
            capLetter = 'a' + (rand() % 26);

        printf("%c", capLetter);

        fputc(capLetter, fp);
    }
    fclose(fp);
    FILE * fp2 = fopen("./prevpwd.txt", "rt");

    fgets(pass, length+1, fp2);
    printf("\nPWD : %s\n", pass);

    fclose(fp2);
    remove("./prevpwd.txt");
    return strdup(pass);
}