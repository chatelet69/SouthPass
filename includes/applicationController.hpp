#ifndef APPLICATION_CONTROLLER_H
#define APPLICATION_CONTROLLER_H

#include <QObject>
#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QStackedWidget>
#include "./db.h"
#include "./credentialsWidget.hpp"
#include "./loginPage.hpp"
#include "./pwdGeneratorPage.h"
#include "./pwdQuality.h"
#include "./dataLeak.hpp"
#include "./parametersPage.hpp"

#define APPLICATION_NAME "SouthPass"

#define WINDOW_MIN_WIDTH 500
#define WINDOW_MIN_HEIGHT 500

#define BUTTON_MAX_WIDTH 150
#define BUTTON_MAX_HEIGHT 50

#define lightModePath "./style/lightMode.css"
#define darkModePath "./style/darkMode.css"
#define lightModeIcon "./assets/lightThemeIcon.png"
#define darkModeIcon "./assets/darkThemeIcon.png"

class MainWindow;
class ParametersPage;
class ApplicationController;
class LoginPage;

class MainWindow : public QMainWindow {
    public:
        MainWindow(QWidget *parent = nullptr);

    private:
        ApplicationController& appController;
};

class ApplicationController : public QObject {
    Q_OBJECT
    public:
        ApplicationController(int argc, char **argv);
        ~ApplicationController();

        int run();
        void changeTheme(QPushButton *themeButton);
        QString getStyleSheet();
        QString getOtherStyleSheet(int darkOrNot);
        QApplication& getApplication();
        int getUserId();
        void deleteChildsOfLayout(QLayout *layout);
        void importHeader(QWidget *headerWidget);

    public Q_SLOTS:
        void importMenu(QMenuBar *);
        void switchToLoginPage();
        void switchCredsPage();
        void switchGenPwdPage();
        void disconnect();
        void switchPwdQuality();
        void exportPasswords();
        void importPasswords();
        void switchLeaksPage();
        void switchParamsPage();

    private:
        int userId;
        char isDark;
        char oldTheme;
        QApplication app;
        QStackedWidget *stackedWidget;
        MainWindow mainWindow;
        MYSQL *dbCon;
        LoginPage *logPage;
        PwdQualityPage *pwdQual;
        PwdGenerator *pwdGen;
        CredentialsPage *credsPage;
        DataLeaksPage *dataLeaksPage;
        ParametersPage *paramsPage;

        void initNullMembers();
        void loadOncePages();
};
#endif