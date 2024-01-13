//
// Created by mathf on 08/01/2024.
//
#include <QLineEdit>
#include <QFormLayout>
#include <QApplication>
#include <QCheckBox>
#include <QMessageBox>
#include <QWidget>
#include "../../includes/applicationController.h"
#include "../../includes/pwdQuality.h"
#include "../../includes/backPwdQuality.h"
#include "../../includes/pincludes.h"

PwdQualityPage::PwdQualityPage(QWidget *parent, ApplicationController *appController, MYSQL *dbCon) : QWidget(parent), dbCon(dbCon) {
    QVBoxLayout * fenetre = new QVBoxLayout();
    QTabWidget *onglets = new QTabWidget(this);

    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QRect screenDimensions = primaryScreen->availableGeometry();
/*
    int x = screenDimensions.width() / 3.1;
    int y = screenDimensions.height() / 2;
    onglets->setMinimumSize(x, y);
*/
    fenetre->setObjectName("fenetreQuality");
    verifWeakPwd(onglets);
    weakPwdList(onglets);
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

    free(start);

    onglets->addTab(weakList, "Liste mots de passes faibles");
}


void PwdQualityPage::reUsedPwd(QTabWidget *onglets){
    QWidget *UsedPwd = new QWidget;

    onglets->addTab(UsedPwd, "Mots de passe réutilisés");
}