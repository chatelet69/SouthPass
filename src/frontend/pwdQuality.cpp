//
// Created by mathf on 08/01/2024.
//
#include <QLineEdit>
#include <QFormLayout>
#include <QApplication>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <cstdlib>
#include <stdio.h>
#include <QWidget>
#include "../../includes/applicationController.hpp"
#include "../../includes/pwdQuality.h"
#include "../../includes/backPwdQuality.h"
#include "../../includes/fileController.h"
#include "../../includes/pincludes.h"

PwdQualityPage::PwdQualityPage(QWidget *parent, ApplicationController *appController, MYSQL *dbCon) : QWidget(parent), dbCon(dbCon) {
    QVBoxLayout * fenetre = new QVBoxLayout(this);
    QTabWidget *onglets = new QTabWidget(this);
    fenetre->setObjectName("fenetreQuality");
    verifWeakPwd(onglets);
    weakPwdList(onglets);
    reUsedPwd(onglets);
    fenetre->addWidget(onglets);
}

void PwdQualityPage::verifWeakPwd(QTabWidget *onglets){
    QWidget *verifWeaknessPwd = new QWidget;
    verifWeaknessPwd->setObjectName("backPwdQuality");
    QLabel *verifTitle = new QLabel();
    verifTitle->setText("Vérifier votre mot de passe  :");
    verifTitle->setAlignment(Qt::AlignCenter);
    verifTitle->setObjectName("verifTitle");

    QFormLayout *layoutweakPwd = new QFormLayout;
    QHBoxLayout * horizontalForm = new QHBoxLayout;
    layoutweakPwd->setObjectName("layoutForm");
    layoutweakPwd->addRow(verifTitle);
    QLineEdit *pwdInput = new QLineEdit(this);
    pwdInput->setObjectName("pwdInput");
    QPushButton *validButton = new QPushButton("Tester", this);
    horizontalForm->addWidget(pwdInput);
    horizontalForm->addWidget(validButton);
    layoutweakPwd->addRow(horizontalForm);
    // layoutweakPwd->addRow(validButton);
    QLabel *solidityPwd = new QLabel();
    solidityPwd->setText("Solidité du mot de passe :");
    solidityPwd->setObjectName("solidityPwd");
    layoutweakPwd->addRow(solidityPwd);
    verifWeaknessPwd->setLayout(layoutweakPwd);
    onglets->addTab(verifWeaknessPwd, "Solidité mot de passe");

    connect(validButton, &QPushButton::clicked, [=](){
        char pwd[100];
        QByteArray futurPwd = pwdInput->text().toLocal8Bit();
        strcpy(pwd, futurPwd.data());
        if(pwd[strlen(pwd)-1] == '\n')
            pwd[strlen(pwd)-1] = '\0';
        char score[50];
        strcpy(score, testPwd(pwd));

        char solidity[40] = "Solidité du mot de passe : ";
        strcat(solidity, score);

        solidityPwd->setText(solidity);
        verifWeaknessPwd->setLayout(layoutweakPwd);
    });
}


void PwdQualityPage::weakPwdList(QTabWidget *onglets){
    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);

    QWidget *weakList = new QWidget();
    QVBoxLayout * mainLayout = new QVBoxLayout(weakList);
    weakList->setObjectName("backPwdQuality");

    struct WeakPwdList *start = NULL;
    start = getAllWeaksPwd(start, dbCon);

    if(start != NULL){
        while(start!=NULL){
            QWidget * weakPwdBox = new QWidget();
            weakPwdBox->setObjectName("boxPwdQuality");

            QHBoxLayout * weakPwdHLayout = new QHBoxLayout();
            QVBoxLayout * weakInfosLayout = new QVBoxLayout();

            char url[200];
            sprintf(url, "url : %s", start->site);
            QLabel *urlOfWeakPwd = new QLabel();
            urlOfWeakPwd->setText(url);
            char login[200];
            sprintf(login, "login : %s", start->username);
            QLabel *loginOfWeakPwd = new QLabel();
            loginOfWeakPwd->setText(login);
            char weakPwd[200];
            sprintf(weakPwd, "pwd : %s", start->pwd);
            QLabel *WeakPwdLabel = new QLabel();
            WeakPwdLabel->setText(weakPwd);
            start = start->next;

            weakInfosLayout->addWidget(urlOfWeakPwd);
            weakInfosLayout->addWidget(loginOfWeakPwd);
            weakInfosLayout->addWidget(WeakPwdLabel);

            QPushButton *editWeakPwd = new QPushButton("Edit");
            weakPwdHLayout->addLayout(weakInfosLayout);
            weakPwdHLayout->addWidget(editWeakPwd);
            mainLayout->addWidget(weakPwdBox);
            weakPwdBox->setLayout(weakPwdHLayout);
        }
    }else{
        // printf("\nListe vide.");
        QLabel *noWeakPwd = new QLabel("Vous n'avez aucun mot de passe faible !");
        mainLayout->addWidget(noWeakPwd);
    }
    scrollArea->setWidget(weakList);
    onglets->addTab(scrollArea, "Liste mots de passes faibles");
    free(start);
}

void PwdQualityPage::reUsedPwd(QTabWidget *onglets){
    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);

    QWidget *reUsedList = new QWidget;
    QVBoxLayout *reUsedLayout = new QVBoxLayout(reUsedList);  // Utiliser reUsedList comme parent du layout
    reUsedList->setObjectName("backPwdQuality");

    TokenInfos *tokenInfos = getTokenFileInfos();
    if(tokenInfos != NULL || tokenInfos -> id != NULL) {
        struct PwdList *pwds = getUniquePwd(dbCon, tokenInfos->id);
        if(pwds != NULL) {
            if (pwds != NULL || dbCon != NULL) {
                for (unsigned int i = 0; i < pwds->size; ++i) {
                    if (pwds->pwd[i] != NULL) {
                        struct WebsiteByPwd *websites = getWebsiteByPwd(dbCon, pwds->pwd[i], tokenInfos->id);
                        if (websites != NULL && websites->website != NULL) {
                            if (websites->size >= 2) {
                                QHBoxLayout * nbWebsiteByPwd = new QHBoxLayout();
                                char webByPwd[200];
                                sprintf(webByPwd, "%d comptes utilisent le même mot de passe : ", websites->size);
                                QLabel * displayNbWebsites = new QLabel();
                                displayNbWebsites->setAlignment(Qt::AlignCenter);
                                displayNbWebsites->setText(webByPwd);
                                displayNbWebsites->setObjectName("displayNbWebsites");
                                nbWebsiteByPwd->addWidget(displayNbWebsites);
                                reUsedLayout->addLayout(nbWebsiteByPwd);

                                for (unsigned int j = 0; j < websites->size; ++j) {
                                    QWidget * boxPwd = new QWidget();
                                    QHBoxLayout * boxPwdLayout = new QHBoxLayout();
                                    boxPwd->setObjectName("boxPwdQuality");

                                    QWidget * webInfos = new QWidget();
                                    QVBoxLayout * webInfosLayout = new QVBoxLayout();

                                    char url[200];
                                    sprintf(url, "url : %s", websites->website[j].website);
                                    QLabel * urlDisplay = new QLabel();
                                    urlDisplay->setText(url);
                                    char login[200];
                                    sprintf(login, "login : %s", websites->website[j].username);
                                    QLabel * displayLogin = new QLabel();
                                    displayLogin->setText(login);
                                    char pwd[200];
                                    sprintf(pwd, "pwd : %s", pwds->pwd[i]);
                                    QLabel * displayPwd = new QLabel();
                                    displayPwd->setText(pwd);

                                    webInfosLayout->addWidget(urlDisplay);
                                    webInfosLayout->addWidget(displayLogin);
                                    webInfosLayout->addWidget(displayPwd);

                                    QPushButton * editPwd = new QPushButton("Edit");
                                    // QVBoxLayout * editInfos = new QVBoxLayout();
                                    // editInfos->addWidget(editPwd);
                                    boxPwdLayout->addLayout(webInfosLayout);
                                    boxPwdLayout->addWidget(editPwd);
                                    reUsedLayout->addWidget(boxPwd);
                                    boxPwd->setLayout(boxPwdLayout);
                                }

                                free(websites->website);
                                free(websites);
                            }
                        }
                    }
                }
                for (unsigned int j = 0; j < pwds->size; ++j) {
                    free(pwds->pwd[j]);
                }
                free(pwds);

                scrollArea->setWidget(reUsedList);
            }
        }
    }
    free(tokenInfos->token);
    free(tokenInfos->email);
    free(tokenInfos);
    onglets->addTab(scrollArea, "Mots de passes réutilisés");
}
