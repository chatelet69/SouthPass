//
// Created by mathf on 08/01/2024.
//
#include <QLineEdit>
#include <QFormLayout>
#include <stdlib.h>
#include <stdio.h>
#include <QApplication>
#include <QCheckBox>
#include <QMessageBox>
#include <QWidget>
#include "../../includes/applicationController.h"
#include "../../includes/pwdQuality.h"
#include "../../includes/backPwdQuality.h"
#include "../../includes/fileController.h"
#include "../../includes/pincludes.h"

PwdQualityPage::PwdQualityPage(QWidget *parent, ApplicationController *appController, MYSQL *dbCon) : QWidget(parent), dbCon(dbCon) {
    QVBoxLayout * fenetre = new QVBoxLayout();
    QTabWidget *onglets = new QTabWidget(this);

    fenetre->setObjectName("fenetreQuality");
    verifWeakPwd(onglets);
    weakPwdList(onglets);
    reUsedPwd(onglets);
    fenetre->addWidget(onglets);
}

void PwdQualityPage::verifWeakPwd(QTabWidget *onglets){
    QWidget *verifWeaknessPwd = new QWidget;
    QLabel *verifTitle = new QLabel();
    verifTitle->setText("Vérifier la solidité d'un mot de passe :");

    QFormLayout *layoutweakPwd = new QFormLayout;

    layoutweakPwd->setObjectName("layoutForm");
    layoutweakPwd->addRow(verifTitle);
    QLineEdit *pwdInput = new QLineEdit;
    layoutweakPwd->addRow("Mot de passe à tester :", pwdInput);
    QPushButton *validButton = new QPushButton("Tester");
    layoutweakPwd->addRow(validButton);
    QLabel *solidityPwd = new QLabel();
    QLabel *advice = new QLabel();
    layoutweakPwd->addRow(solidityPwd);
    layoutweakPwd->addRow(advice);
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
        char advicePwd[140] = "Conseil : Essayez d'avoir un mot de passe de 25 caractères qui comporte des lettres majuscule et minuscule, des chiffres et des symboles.";

        solidityPwd->setText(solidity);
        advice->setText(advicePwd);
        verifWeaknessPwd->setLayout(layoutweakPwd);
    });
}


void PwdQualityPage::weakPwdList(QTabWidget *onglets){
    QWidget *weakList = new QWidget;
    QLabel *listTitle = new QLabel();
    listTitle->setText("Mots de passes faibles :");
    struct WeakPwdList *start = NULL;
    start = getAllWeaksPwd(start, dbCon);
    /*
     * start = début de la liste des pwd faible avec url, username et pwd dans cette liste
     * Faire l'affichage
     */
    free(start);

    onglets->addTab(weakList, "Liste mots de passes faibles");
}

void PwdQualityPage::reUsedPwd(QTabWidget *onglets){
    QWidget *weakList = new QWidget;
    QLabel *listTitle = new QLabel();
    listTitle->setText("Mots de passes réutilisés :");

    TokenInfos *tokenInfos = getTokenFileInfos();
    struct PwdList *pwds = getUniquePwd(dbCon, tokenInfos->id);
    for (int i = 0; i < pwds->size; ++i) {
        printf("\n\n      Sites ayant le mot de passe : %s i : %d", pwds[i].pwd, i);
        struct WebsiteByPwd *websites = getWebsiteByPwd(dbCon, pwds[i].pwd, tokenInfos->id);
        for (int j = 0; j < websites->size; ++j) {
            printf("\nSite : %s, login : %s", websites[j].website, websites[j].username);
        }
        printf("\n");
        for (int j = 0; j < websites->size; ++j) {
            free(websites[j].website);
        }
    }
    for (int j = 0; j < pwds->size; ++j) {
        free(pwds[j].pwd);
    }
/*
    struct ReUsedPwdByWebsite *start = NULL;
    start = getReUsedPwd(start,dbCon);
    printReUsedPwd(start);
*/
    /*
     * start = début de la liste des pwd faible avec url, username et pwd dans cette liste
     * Faire l'affichage
     */

    //    free(start);
    onglets->addTab(weakList, "Liste mots de passes faibles");
}