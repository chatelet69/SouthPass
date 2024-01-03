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

class loginPage : public QWidget {
public:
    loginPage(QWidget *parent, MYSQL *dbCon);
    //~CredentialsPage();

public Q_SLOTS:
    // void showForm();

private Q_SLOTS:
    //void showCredsForm();

private:
    MYSQL *dbCon;
    QWidget fenetre;
    QLineEdit *email;
    QLineEdit *password;
    QFormLayout *layout;
};
#endif //SOUTHPASS_LOGIN_PAGE_H
