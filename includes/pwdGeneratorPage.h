//
// Created by mathf on 08/01/2024.
//

#ifndef SOUTHPASS_PWDGENERATORPAGE_H
#define SOUTHPASS_PWDGENERATORPAGE_H
#include "db.h"
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFormLayout>
#include "applicationController.hpp"
class ApplicationController;

class PwdGenerator : public QWidget {
Q_OBJECT
public:
    PwdGenerator(QWidget *parent,ApplicationController *, MYSQL *dbCon);

public Q_SLOTS:

private Q_SLOTS:

private:
    MYSQL *dbCon;
    QWidget window;
};
#endif //SOUTHPASS_PWDGENERATORPAGE_H
