//
// Created by mathf on 19/01/2024.
//

#ifndef SOUTHPASS_PARAMETERSPAGE_H
#define SOUTHPASS_PARAMETERSPAGE_H
#include "db.h"
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFormLayout>
#include "./applicationController.hpp"

class ApplicationController;
void changeThemeMode(ApplicationController *appController, QApplication *app);

class ParametersPage : public QWidget {
Q_OBJECT
public:
    ParametersPage(ApplicationController *appController, QApplication *app, MYSQL *dbCon);
    static char * convertStringForC(QLineEdit * string);

public Q_SIGNAL:

public Q_SLOTS:

private Q_SLOTS:

private:
    MYSQL *dbCon;
    QScrollArea *scrollArea;
};
#endif //SOUTHPASS_PARAMETERSPAGE_H
