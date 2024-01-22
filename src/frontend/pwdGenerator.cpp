/*
    Filename : pwdGenerator.cpp
    Description : Password Generator Page
*/

#include "../../includes/pincludes.h"
#include "../../includes/pwdGenController.h"
#include <QAbstractSlider>
#include <QSpinBox>
#include <string.h>
#include <stdio.h>
#include <QCheckBox>

PwdGenerator::PwdGenerator(QWidget *parent,ApplicationController *, MYSQL *dbCon){
    QVBoxLayout * window = new QVBoxLayout(this);
    QWidget * mainWidget = new QWidget();   
    mainWidget->setObjectName("genPwdMain");
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    QFormLayout *layoutPwdGen = new QFormLayout;

    QLabel *pwdGenTitle = new QLabel();
    pwdGenTitle->setText("Générer un mot de passe :");
    pwdGenTitle->setAlignment(Qt::AlignCenter);
    pwdGenTitle->setObjectName("genPwdTitle");
    QSpinBox *lengthPwd = new QSpinBox();
    lengthPwd->setMinimum(8);
    lengthPwd->setMaximum(40);
    lengthPwd->setValue(25);
    lengthPwd->setSingleStep(1);

    QCheckBox *useMajChars = new QCheckBox("Utiliser des lettres majuscules (A-Z)");
    QCheckBox *useNumbers = new QCheckBox("Utiliser des chiffres (0-9)");
    QCheckBox *useSymbols = new QCheckBox("Utiliser des symboles (@!$%&*)");
    QPushButton *genPwdBtn = new QPushButton("Générer le mot de passe");


    mainLayout->addWidget(pwdGenTitle);
    layoutPwdGen->addRow("Longueur du mot de passe :", lengthPwd);
    layoutPwdGen->addRow(useMajChars);
    layoutPwdGen->addRow(useNumbers);
    layoutPwdGen->addRow(useSymbols);
    layoutPwdGen->addRow(genPwdBtn);
    mainLayout->addLayout(layoutPwdGen);

    QLabel *titleResultPwd = new QLabel("Mot de passe généré :");
    mainLayout->addWidget(titleResultPwd);
    // mainWidget->setLayout(mainLayout);
    window->addWidget(mainWidget);
    setLayout(window);
    connect(genPwdBtn, &QPushButton::clicked, this, [=](){

        int verifMaj, verifNum, verifSymbols, length;

        useMajChars->isChecked() ? verifMaj = 1 : verifMaj = 0;
        useNumbers->isChecked() ? verifNum = 1 : verifNum = 0;
        useSymbols->isChecked() ? verifSymbols = 1 : verifSymbols = 0;
        length = lengthPwd->value();

        char result[200];
        sprintf(result, "Mot de passe généré : %s", generatePwd(length, verifMaj, verifNum, verifSymbols));
        titleResultPwd->setText(result);
        setLayout(window);
        return 0;
    });
}