#ifndef SOUTHPASS_PARAMETERSPAGE_HPP
#define SOUTHPASS_PARAMETERSPAGE_HPP

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFormLayout>
#include "applicationController.hpp"
#include "db.h"

class ApplicationController;
class ParametersPage;

void changeThemeMode(ApplicationController *appController, QApplication *app);

class ParametersPage : public QWidget {
    Q_OBJECT
    public:
        ParametersPage(ApplicationController *appController, QApplication *app, MYSQL *dbCon);
        static char * convertStringForC(QLineEdit * string);
        void importSecurityParameter(QWidget *secuBox);
        void importEmailParameter(QWidget *emailBox);

    public Q_SIGNAL:

    public Q_SLOTS:
        void showEditEmailBox(QWidget *parametersParent);
        void saveNewEmail(QString emailValue);

    private:
        MYSQL *dbCon;
        QScrollArea *scrollArea;

        QString userEmail;
        int userId;
};

#endif //SOUTHPASS_PARAMETERSPAGE_HPP
