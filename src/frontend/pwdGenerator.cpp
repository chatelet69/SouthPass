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
    QLabel *pwdGenTitle = new QLabel(this);
    pwdGenTitle->setText("Générateur mot de passe :");
    QFormLayout *layoutPwdGen = new QFormLayout;
    QSpinBox *lengthPwd = new QSpinBox();
    lengthPwd->setMinimum(8);
    lengthPwd->setMaximum(40);
    lengthPwd->setValue(25);
    lengthPwd->setSingleStep(1);

    QCheckBox *useMajChars = new QCheckBox("Utiliser des lettres majuscules (A-Z)", this);
    QCheckBox *useNumbers = new QCheckBox("Utiliser des chiffres (0-9)", this);
    QCheckBox *useSymbols = new QCheckBox("Utiliser des symboles (@!$%&*)", this);
    QPushButton *genPwdBtn = new QPushButton("Générer le mot de passe");


    layoutPwdGen->addRow(pwdGenTitle);
    layoutPwdGen->addRow("Longueur du mot de passe :", lengthPwd);
    layoutPwdGen->addRow(useMajChars);
    layoutPwdGen->addRow(useNumbers);
    layoutPwdGen->addRow(useSymbols);
    layoutPwdGen->addRow(genPwdBtn);
    QLabel *titleResultPwd = new QLabel(this);
    QLabel *resultPwd = new QLabel(this);
    layoutPwdGen->addRow(titleResultPwd);
    layoutPwdGen->addRow(resultPwd);


    setLayout(layoutPwdGen);

    connect(genPwdBtn, &QPushButton::clicked, this, [=](){

        int verifLength, verifMaj, verifNum, verifSymbols, length;

        useMajChars->isChecked() ? verifMaj = 1 : verifMaj = 0;
        useNumbers->isChecked() ? verifNum = 1 : verifNum = 0;
        useSymbols->isChecked() ? verifSymbols = 1 : verifSymbols = 0;
        length = lengthPwd->value();

        titleResultPwd->setText("Mot de passe généré :");
        resultPwd->setText(generatePwd(length, verifMaj, verifNum, verifSymbols));
        setLayout(layoutPwdGen);
        return 0;
    });
}