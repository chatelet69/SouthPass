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
        ParametersPage(ApplicationController *appController, QApplication *app, MYSQL *dbConnection);
        static char * convertStringForC(QLineEdit * string);
        void importSecurityParameter(QWidget *secuBox);
        void importEmailParameter(QWidget *emailBox);

    public Q_SIGNAL:

    public Q_SLOTS:
        void showEditEmailBox(QWidget *parametersParent);
        void saveNewEmail(QString emailValue);
        void showEditAccountPassword(QString passwordType, QString passwordLabelText);
        void saveNewPwd(QString type, QString passValue, QString confirmationPass, QString actualPass);
        void showVerifEmailCode(char *newEmail, char *actualEmail);
        void importVerifEmailWindow(int emailCode, char *email, char *newEmail);
        void saveNewEmailFinalStep(QString code, int emailCode, char *newEmail, char *actualEmail);

    private:
        MYSQL *dbCon;
        int emailCode;
        ApplicationController *appController;
        QScrollArea *scrollArea;

        QString userEmail;
        int userId;

        QPushButton *importCancelButton(QWidget *buttonsContainer, QHBoxLayout *buttonsLayout);
        QPushButton *importSaveButton(QWidget *buttonsContainer, QHBoxLayout *buttonsLayout);
        void showMessageBoxSuccess(QString text);
        void showMessageBoxError(QString text, int errorType);
};

#endif //SOUTHPASS_PARAMETERSPAGE_HPP
