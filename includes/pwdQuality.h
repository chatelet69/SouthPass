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
#include "applicationController.h"
class ApplicationController;

class PwdQualityPage : public QWidget {
Q_OBJECT
public:
    PwdQualityPage(QWidget *parent,ApplicationController *, MYSQL *dbCon);

public Q_SIGNAL:

public Q_SLOTS:

private Q_SLOTS:

private:
    MYSQL *dbCon;
};
#endif //SOUTHPASS_PWDQUAL_H