/*
    Filename : loginPage.cpp
    Description : App login and registration page
    Last Edit : 20_01_2024
*/

#include <QLineEdit>
#include <QFormLayout>
#include <QApplication>
#include <QCheckBox>
#include <QMessageBox>
#include "../../includes/applicationController.hpp"
#include "../../includes/backLoginSignIn.h"
#include "../../includes/pincludes.h"

LoginPage::LoginPage(QWidget *parent, ApplicationController *appController, MYSQL *dbCon) : QWidget(parent), dbCon(dbCon) {
    //Création des onglets connexion et inscription
    QVBoxLayout * logWindow = new QVBoxLayout(this);
    QTabWidget *onglets = new QTabWidget(this);

    // Page connexion
    QWidget *loginBox = new QWidget();
    loginBox->setObjectName("loginBox");
    QVBoxLayout * loginLayout = new QVBoxLayout(loginBox);
    QLabel *loginLabelTitle = new QLabel("Connexion :");
    loginLabelTitle->setAlignment(Qt::AlignCenter);

    loginLabelTitle->setObjectName("loginLabelTitle");
    QFormLayout *formLogin = new QFormLayout;
    QLabel *emailLabel = new QLabel("Email :");
    emailLabel->setObjectName("loginLabels");
    QLineEdit *loginEmail = new QLineEdit;
    loginEmail->setObjectName("inputLogin");
    QLabel *pwdLabel = new QLabel("Mot de passe :");
    pwdLabel->setObjectName("loginLabels");
    QLineEdit *loginPassword = new QLineEdit;
    loginPassword->setObjectName("inputLogin");
    loginPassword->setEchoMode(QLineEdit::Password);
    QLabel *masterPwdLabel = new QLabel("Mot de passe maitre:");
    masterPwdLabel->setObjectName("loginLabels");
    QLineEdit *loginMasterPassword = new QLineEdit;
    loginMasterPassword->setObjectName("inputLogin");
    loginMasterPassword->setEchoMode(QLineEdit::Password);
    QPushButton *connectButton = new QPushButton("Se connecter");
    // le symbole « & » permet de définir des raccourcis clavier
    formLogin->addRow(loginLabelTitle);
    formLogin->addRow(emailLabel);
    formLogin->addRow(loginEmail);
    formLogin->addRow(pwdLabel);
    formLogin->addRow(loginPassword);
    formLogin->addRow(masterPwdLabel);
    formLogin->addRow(loginMasterPassword);
    formLogin->addRow(connectButton);
    loginLayout->addLayout(formLogin);
    loginBox->setLayout(loginLayout);

    // Page inscription
    QWidget *signInBox = new QWidget;
    signInBox->setObjectName("loginBox");
    QLabel *signInLabel = new QLabel("Inscription :");
    signInLabel->setAlignment(Qt::AlignCenter);
    signInLabel->setObjectName("loginLabelTitle");
    QFormLayout *layoutSignIn = new QFormLayout;
    QLabel *emailTitle = new QLabel("Email :");
    emailTitle->setObjectName("loginLabels");
    QLabel *pwdTitle = new QLabel("Mot de passe :");
    pwdTitle->setObjectName("loginLabels");
    QLabel *confirmPwdTitle = new QLabel("Confirmation mot de passe :");
    confirmPwdTitle->setObjectName("loginLabels");
    QLabel *pwdMasterTitle = new QLabel("Mot de passe maitre :");
    pwdMasterTitle->setObjectName("loginLabels");
    QLabel *confirmPwdMasterTitle = new QLabel("Confirmation mot de passe :");
    confirmPwdMasterTitle->setObjectName("loginLabels");

    QLineEdit *signEmail = new QLineEdit;
    signEmail->setObjectName("inputLogin");
    QLineEdit *signPassword = new QLineEdit;
    signPassword->setEchoMode(QLineEdit::Password);
    signPassword->setObjectName("inputLogin");
    QLineEdit *confirmPassword = new QLineEdit;
    confirmPassword->setEchoMode(QLineEdit::Password);
    confirmPassword->setObjectName("inputLogin");
    QLineEdit *signMasterPwd = new QLineEdit;
    signMasterPwd->setEchoMode(QLineEdit::Password);
    signMasterPwd->setObjectName("inputLogin");
    QLineEdit *confirmMasterPwd = new QLineEdit;
    confirmMasterPwd->setEchoMode(QLineEdit::Password);
    confirmMasterPwd->setObjectName("inputLogin");

    layoutSignIn->addRow(signInLabel);
    layoutSignIn->addRow(emailTitle);
    layoutSignIn->addRow(signEmail);
    layoutSignIn->addRow(pwdTitle);
    layoutSignIn->addRow(signPassword);
    layoutSignIn->addRow(confirmPwdTitle);
    layoutSignIn->addRow(confirmPassword);
    layoutSignIn->addRow(pwdMasterTitle);
    layoutSignIn->addRow(signMasterPwd);
    layoutSignIn->addRow(confirmPwdMasterTitle);
    layoutSignIn->addRow(confirmMasterPwd);
    QPushButton *signInButton = new QPushButton("S'inscrire");
    layoutSignIn->addRow(signInButton);
    signInBox->setLayout(layoutSignIn);


    // Ajout des onglets Connexion et Inscription
    onglets->addTab(loginBox, "Connexion");
    onglets->addTab(signInBox, "Inscription");
    logWindow->addWidget(onglets);

    /* CSS : ( à mettre dans les fonctions )
    * login->setObjectName("loginTab");
    *login->setObjectName("loginWidget");
    *signIn->setObjectName("signInWidget");
    */

    // Intéraction bouton login
    connectButton->setObjectName("connectButton");
    connect(connectButton, &QPushButton::clicked, [=](){
        // CONVERSION DES INPUT EN STRING POUR LE C
        char logMail[50];
        QByteArray futurLogMail = loginEmail->text().toLocal8Bit();
        strcpy(logMail, futurLogMail.data());
        if(logMail[strlen(logMail)-1] == '\n')
            logMail[strlen(logMail)-1] = '\0';
        char logPwd[50];
        QByteArray futurLogPwd = loginPassword->text().toLocal8Bit();
        strcpy(logPwd, futurLogPwd.data());
        if(logPwd[strlen(logMail)-1] == '\n')
            logPwd[strlen(logMail)-1] = '\0';
        char logMasterPwd[50];
        QByteArray futurLogMasterPwd = loginMasterPassword->text().toLocal8Bit();
        strcpy(logMasterPwd, futurLogMasterPwd.data());
        if(logMasterPwd[strlen(logMasterPwd)-1] == '\n')
            logMasterPwd[strlen(logMasterPwd)-1] = '\0';

        int res;
        res = verifLogin(dbCon, logMail, logPwd, logMasterPwd);
        if(res == 1){
            QMessageBox::warning(this,"Erreur" ,"Email ou mots de passes incorrects veuillez re essayer.");
        }else if(res == 0){
            QMessageBox::information(this,"Bravo !" ,"Connexion réussie !");
            appController->switchCredsPage();
            return 0;
        }
        return 1;
    });

    // Intéraction bouton Inscription
    signInButton->setObjectName("signInButton");
    connect(signInButton, &QPushButton::clicked, [=](){
        // conversion mail de Qtring à char * pour le backend en C
        char signMail[50];
        QByteArray futurStringMail = signEmail->text().toLocal8Bit();
        strcpy(signMail, futurStringMail.data());
        // conversion password de Qtring à char * pour le backend en C
        char password[50];
        QByteArray futurStringPwd = signPassword->text().toLocal8Bit();
        strcpy(password, futurStringPwd.data());
        // conversion verifPassword de Qtring à char * pour le backend en C
        char verifPassword[50];
        QByteArray futurStringConfirmPwd = confirmPassword->text().toLocal8Bit();
        strcpy(verifPassword, futurStringConfirmPwd.data());
        if(verifPassword[strlen(verifPassword)-1] == '\n')
            verifPassword[strlen(verifPassword)-1] = '\0';
        // conversion signMasterPwd de Qtring à char * pour le backend en C
        char masterPwd[50];
        QByteArray futurMasterPwd = signMasterPwd->text().toLocal8Bit();
        strcpy(masterPwd, futurMasterPwd.data());
        if(masterPwd[strlen(masterPwd)-1] == '\n')
            masterPwd[strlen(masterPwd)-1] = '\0';
        // conversion confirmMasterPwd de Qtring à char * pour le backend en C
        char verifMasterPassword[50];
        QByteArray futurMasterConfirmPwd = confirmMasterPwd->text().toLocal8Bit();
        strcpy(verifMasterPassword, futurMasterConfirmPwd.data());
        if(verifMasterPassword[strlen(verifMasterPassword)-1] == '\n')
            verifMasterPassword[strlen(verifMasterPassword)-1] = '\0';

        if(strcmp(verifSignIn(signMail, password, verifPassword,masterPwd, verifMasterPassword), "ok")==0){
            int res;
            res = createUser(dbCon, signMail, password, masterPwd);
            if(res == 0){
                QMessageBox::information(this,"Bravo !" ,"Inscription réussie !");
                appController->switchCredsPage();
                // emit signInSuccess();
                return 0;
            }else if(res == 2){
                QMessageBox::warning(this,"Erreur" ,"Email déjà utilisé, essayez de vous connecter");
            }else{
                QMessageBox::warning(this,"Erreur" ,"Erreur lors de la création du compte");
            }
        }else{
            QLabel *signInLabel = new QLabel(this);
            signInLabel->setText(verifSignIn(signMail, password, verifPassword, masterPwd, verifMasterPassword));
            QMessageBox::warning(this,"Erreur" ,verifSignIn(signMail, password, verifPassword, masterPwd, verifMasterPassword));
        }
        return 1;
    });
}


void LoginPage::signInSuccess(){
    printf("Succès");
}