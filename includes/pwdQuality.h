//
// Created by mathf on 03/01/2024.
//
#ifndef SOUTHPASS_PWDQUAL_H
#define SOUTHPASS_PWDQUAL_H

#include "db.h"
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMessageBox>
#include <QFormLayout>
#include <QDialog>
#include "applicationController.hpp"
#include <QString>
#include "backPwdQuality.h"

class ApplicationController;

class PwdQualityPage : public QWidget {
Q_OBJECT
public:
    PwdQualityPage(QStackedWidget *parent,ApplicationController *, MYSQL *dbCon);

public Q_SIGNAL:

public Q_SLOTS:
    void verifWeakPwd(QTabWidget *onglets);
    void weakPwdList(QStackedWidget * parent, QTabWidget *onglets);
    void reUsedPwd(QStackedWidget * parent, QTabWidget *onglets);
private Q_SLOTS:

private:
    MYSQL *dbCon;
    QVBoxLayout *contentLayout;
    QScrollArea *scrollArea;
    CredentialEditWidget *credentialEditWidget;
};

class EditPwd : public QDialog {
    Q_OBJECT
public:
    EditPwd(MYSQL *dbCon, QStackedWidget *parent = nullptr, char * url = NULL, char * login = NULL, char * pwd = NULL, int id = 0, int userId = 0) : QDialog(parent){
        initQDialog(dbCon, parent, url, login, pwd, id, userId);
    }
private:
    void initQDialog(MYSQL * dbCon, QStackedWidget *parent, char * url, char * login, char * pwd, int id, int userId){
        QLabel *label = new QLabel("Changer de mot de passe :");
        QFormLayout * form = new QFormLayout();
        QLabel *newPwdLabel = new QLabel("Nouveau mot de passe :");
        QLineEdit *newPwd = new QLineEdit();
        newPwd->setEchoMode(QLineEdit::Password);
        QLabel *confirmPwdLabel = new QLabel("Nouveau mot de passe :");
        QLineEdit *confirmPwd = new QLineEdit();
        confirmPwd->setEchoMode(QLineEdit::Password);
        QPushButton *closeBtn = new QPushButton("Annuler", this);
        closeBtn->setObjectName("cancelButton");
        QPushButton *changeBtn = new QPushButton("Valider", this);
        closeBtn->setObjectName("cancelButton");
        connect(closeBtn, &QPushButton::clicked, this, &QDialog::reject); // Utilise reject() pour fermer la QDialog
        connect(changeBtn, &QPushButton::clicked, this, [=](){
            if(newPwd->text().toUtf8().constData() == NULL || confirmPwd->text().toUtf8().constData() == NULL){
                QMessageBox::warning(this, "Erreur", "Veuillez renseigner les mots de passe");
                return 1;
            }
            const char *newPwdC = newPwd->text().toUtf8().constData();
            const char *confirmPwdC = confirmPwd->text().toUtf8().constData();
            int res;
            res = verifEditPwd(newPwdC, confirmPwdC);
            if(res != 0) {
                QMessageBox::warning(this, "Erreur", "Mot de passes différents.");
                return 1;
            }
            char * pwdNew = newPwd->text().toUtf8().data();
            res = updateWebsitePwd(dbCon, pwdNew, id, userId);
            if(res == 1){
                QMessageBox::warning(this, "Erreur", "Erreur lors de la modification du mot de passe.");
                return 1;
            }
            PwdQualityPage* pwdQual = new PwdQualityPage(parent, reinterpret_cast<ApplicationController *>(this), dbCon);
            parent->addWidget(pwdQual);
            parent->setCurrentWidget(pwdQual);
            this->hide();
            this->close();
            return 0;
        });

        QVBoxLayout *layoutPrincipal = new QVBoxLayout(this);
        QWidget * mainWidget = new QWidget();
        QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
        QHBoxLayout *layoutBtns = new QHBoxLayout();

        form->addRow(label);
        form->addRow(newPwdLabel);
        form->addRow(newPwd);
        form->addRow(confirmPwdLabel);
        form->addRow(confirmPwd);
        layoutBtns->addWidget(closeBtn);
        layoutBtns->addWidget(changeBtn);
        form->addRow(layoutBtns);

        mainLayout->addLayout(form);

        layoutPrincipal->addWidget(mainWidget);

        setLayout(layoutPrincipal);

        setWindowTitle("Modifier votre mot de passe");
        setFixedSize(300, 250);
    }
};

#endif //SOUTHPASS_PWDQUAL_H
