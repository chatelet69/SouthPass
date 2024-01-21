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
#include <QFormLayout>
#include "applicationController.hpp"
class ApplicationController;

void editPwdWeak(QWidget *parent, ApplicationController *appController, MYSQL * dbCon, int id,  char * pwd, char *username, char *site, QPushButton *editWeakPwd);
class PwdQualityPage : public QWidget {
Q_OBJECT
public:
    PwdQualityPage(QWidget *parent,ApplicationController *, MYSQL *dbCon);

public Q_SIGNAL:

public Q_SLOTS:
    void verifWeakPwd(QTabWidget *onglets);
    void weakPwdList(QTabWidget *onglets);
    void reUsedPwd(QTabWidget *onglets);
private Q_SLOTS:

private:
    MYSQL *dbCon;
    QVBoxLayout *contentLayout;
    QScrollArea *scrollArea;
    CredentialEditWidget *credentialEditWidget;
};
#endif //SOUTHPASS_PWDQUAL_H
