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
#include "applicationController.hpp"
class ApplicationController;

class LoginPage : public QWidget {
    Q_OBJECT
    public:
        LoginPage(QWidget *parent,ApplicationController *, MYSQL *dbCon);

    public Q_SIGNAL:

    public Q_SLOTS:

private:
    MYSQL *dbCon;
};
#endif //SOUTHPASS_LOGIN_PAGE_H
