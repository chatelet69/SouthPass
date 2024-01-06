//
// Created by mathf on 03/01/2024.
//

#ifndef SOUTHPASS_LOGIN_PAGE_H
#define SOUTHPASS_LOGIN_PAGE_H

#include "db.h"
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFormLayout>
#include "applicationController.h"
class ApplicationController;

class loginPage : public QWidget {
Q_OBJECT
public:
    loginPage(QWidget *parent,ApplicationController *, MYSQL *dbCon);

public Q_SIGNAL:
    static void signInSuccess();

public Q_SLOTS:

private Q_SLOTS:

private:
    MYSQL *dbCon;
    QWidget fenetre;
    QLineEdit *email;
    QLineEdit *password;
    QFormLayout *layout;
};
#endif //SOUTHPASS_LOGIN_PAGE_H
