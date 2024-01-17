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
#include <QScrollArea>
#include <QWidget>
#include "../../includes/applicationController.h"
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
    scrollArea = new QScrollArea();

    QWidget *reUsedList = new QWidget;
    QVBoxLayout *verticalLayout = new QVBoxLayout(reUsedList);  // Utiliser reUsedList comme parent du layout

    QVBoxLayout *hLayout = new QVBoxLayout();
    QLabel *listTitle = new QLabel();
    listTitle->setText("Mots de passes réutilisés :");
    hLayout->addWidget(listTitle);
    verticalLayout->addLayout(hLayout);

    TokenInfos *tokenInfos = getTokenFileInfos();
    struct PwdList *pwds = getUniquePwd(dbCon, tokenInfos->id);
    for (unsigned int i = 0; i < pwds->size; ++i) {
        QHBoxLayout *pwdListLayout = new QHBoxLayout();
        char pwdList[200];
        sprintf(pwdList, "Sites ayant le mot de passe : %s", pwds->pwd[i]);
        QLabel *displayPwd = new QLabel();
        displayPwd->setText(pwdList);
        pwdListLayout->addWidget(displayPwd);
        verticalLayout->addLayout(pwdListLayout);
        struct WebsiteByPwd * websites = getWebsiteByPwd(dbCon, pwds->pwd[i], tokenInfos->id);
        for (unsigned int j = 0; j < websites->size; ++j) {
            QHBoxLayout *websiteListLayout = new QHBoxLayout();
            char websiteList[200];
            sprintf(websiteList, "url : %s, login : %s", websites->website[j].website, websites->website[j].username);
            QLabel *displayWeb = new QLabel();
            displayWeb->setText(websiteList);
            websiteListLayout->addWidget(displayWeb);
            verticalLayout->addLayout(websiteListLayout);
        }
        free(websites->website);
        free(websites);

    }
    for (unsigned int j = 0; j < pwds->size; ++j) {
        free(pwds->pwd[j]);
    }
    free(pwds);

    scrollArea->setWidget(reUsedList);
    onglets->addTab(scrollArea, "Mots de passes réutilisés");

}