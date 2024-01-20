//
// Created by mathf on 19/01/2024.
//
#include <QMessageBox>
#include <QRadioButton>
#include <QDesktopServices>
#include "../../includes/applicationController.hpp"
#include "../../includes/parametersPage.h"
#include "../../includes/fileController.h"
#include "../../includes/backParams.h"
char * convertStringForC(QLineEdit * string);

ParametersPage::ParametersPage(ApplicationController *appController, QApplication *app, MYSQL *dbCon){
    QVBoxLayout * fenetre = new QVBoxLayout(this);

    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    QWidget *mainWidget = new QWidget();
    QVBoxLayout * mainLayout = new QVBoxLayout(mainWidget);

    QLabel * accountTitle = new QLabel("Compte :");
    QWidget * emailBox = new QWidget();
    QHBoxLayout * emailLayout = new QHBoxLayout(emailBox);
    TokenInfos *tokenInfos = getTokenFileInfos();
    QVBoxLayout * emailDisplay = new QVBoxLayout();
    QLabel * emailTitle = new QLabel("Email :");
    QLabel * email = new QLabel();


    if(tokenInfos == NULL || tokenInfos->email == NULL)
        email->setText("Erreur");
    else
        email->setText(tokenInfos->email);

    emailDisplay->addWidget(emailTitle);
    emailDisplay->addWidget(email);
    QPushButton * editEmail = new QPushButton("Modifier");
    emailLayout->addLayout(emailDisplay);
    emailLayout->addWidget(editEmail);


    QLabel * secuTitle = new QLabel("Sécurité :");
    QWidget * secuBox = new QWidget();
    QHBoxLayout * secuLayout = new QHBoxLayout(secuBox);
    QLabel * editPwdTitle = new QLabel("Modifier votre mot de passe :");
    QPushButton * editPwdBtn = new QPushButton("Modifier");
    /*QFormLayout *editMasterPwd = new QFormLayout();
    QLineEdit *masterPwdInput = new QLineEdit;
    masterPwdInput->setEchoMode(QLineEdit::Password);
    QLineEdit *verifMasterPwdInput = new QLineEdit;
    verifMasterPwdInput->setEchoMode(QLineEdit::Password);
    QPushButton *editMasterPwdBtn = new QPushButton("Modifier");
    editMasterPwd->addRow(masterPwdInput);
    editMasterPwd->addRow(verifMasterPwdInput);
    editMasterPwd->addRow(editMasterPwdBtn);*/
    secuLayout->addWidget(editPwdTitle);
    secuLayout->addWidget(editPwdBtn);

    QLabel * lookTitle = new QLabel("Apparence :");
    QWidget * darkOrLightBox = new QWidget();
    QHBoxLayout * darkOrLightLayout = new QHBoxLayout(darkOrLightBox);
    QRadioButton * darkMode = new QRadioButton("Dark mode");
    QRadioButton * lightMode = new QRadioButton("Light Mode");
    if(getThemePreference() == 1){
        printf("ok");
        darkMode->setChecked(true);
    }else{
        lightMode->setChecked(true);
        printf("ko");
    }

    darkOrLightLayout->addWidget(darkMode);
    darkOrLightLayout->addWidget(lightMode);

    QLabel * extensionTitle = new QLabel("Extension :");
    QWidget *extensionUrlBox = new QWidget();
    QHBoxLayout * extensionUrlLayout = new QHBoxLayout(extensionUrlBox);
    QLabel * extensionLabel = new QLabel("Installer notre extension web :");
    QPushButton * extUrlBtn = new QPushButton("Installer");
    extensionUrlLayout->addWidget(extensionLabel);
    extensionUrlLayout->addWidget(extUrlBtn);

    mainLayout->addWidget(accountTitle);
    mainLayout->addWidget(emailBox);

    mainLayout->addWidget(secuTitle);
    mainLayout->addWidget(secuBox);

    mainLayout->addWidget(lookTitle);
    mainLayout->addWidget(darkOrLightBox);

    mainLayout->addWidget(extensionTitle);
    mainLayout->addWidget(extensionUrlBox);


    scrollArea->setWidget(mainWidget);
    fenetre->addWidget(scrollArea);
    setLayout(fenetre);

    connect(extUrlBtn, &QPushButton::clicked, [=](){QDesktopServices::openUrl(QUrl("https://www.youtube.com/watch?v=dQw4w9WgXcQ"));});
    connect(darkMode, &QRadioButton::clicked, [=](){changeThemeMode(appController, app);});
    connect(lightMode, &QRadioButton::clicked, [=](){changeThemeMode(appController, app);});
    connect(editPwdBtn, &QPushButton::clicked, [=](){
        // CONVERSION DES INPUT EN STRING POUR LE C
        /*char Pwd[50];
        strcpy(Pwd, ParametersPage::convertStringForC(masterPwdInput));
        char verifPwdString[50];
        strcpy(verifPwdString, ParametersPage::convertStringForC(verifMasterPwdInput));

        int res;
        res = verifPwd(dbCon, Pwd, verifPwdString, tokenInfos->email);
        if(res == 3){
            QMessageBox::warning(this,"Erreur" ,"Une erreur est apparue lors de la modification du mot de passe");
        }else if(res == 2){
            QMessageBox::warning(this,"Erreur" ,"Mots de passes différents, veuillez re essayer.");
        }else if(res == 1){
            QMessageBox::warning(this,"Erreur" ,"Vos mots de passes doivent contenir au moins 1 lettre, 1 chiffre et 1 caractère spécial");
        }else if(res == 0){
            QMessageBox::information(this,"Mot de passe maitre modifié avec succès !" ,"Veuillez vous reconnecter.");
            app->disconnect();
            return 0;
        }
        return 1;*/
    });
}

char * ParametersPage::convertStringForC(QLineEdit * string){
    char stringConverted[50];
    QByteArray futurLogPwd = string->text().toLocal8Bit();
    strcpy(stringConverted, futurLogPwd.data());
    if(stringConverted[strlen(stringConverted)-1] == '\n')
        stringConverted[strlen(stringConverted)-1] = '\0';

    return strdup(stringConverted);
}

void changeThemeMode(ApplicationController *appController, QApplication *app){
    if(getThemePreference() == 1){ //  darkMode -> lightMode
        saveThemePreference(0);
        QString styleSheet = appController->getOtherStyleSheet(0);
        app->setStyleSheet(styleSheet);
    }else{ // lightMode -> darkMode
        saveThemePreference(1);
        QString styleSheet = appController->getOtherStyleSheet(1);
        app->setStyleSheet(styleSheet);
    }

}