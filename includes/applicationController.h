#ifndef APPLICATION_CONTROLLER_H
#define APPLICATION_CONTROLLER_H

#include <QObject>
#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QStackedWidget>
#include "./db.h"
#include "./credentialsWidget.h"
#include "./loginPage.h"
#include "./pwdGeneratorPage.h"
#include "./pwdQuality.h"
#include "./dataLeak.h"

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
class ApplicationController;
class LoginPage;

class MainWindow : public QMainWindow {
    public:
        MainWindow(QWidget *parent = nullptr);
        int getXPos();

    private:
        ApplicationController& appController;
        void onApplicationMove(QMoveEvent *event);
};

class ApplicationController : public QObject {
    Q_OBJECT
    public:
        ApplicationController(int argc, char **argv);
        ~ApplicationController();

        int run();
        void changeTheme(QPushButton *themeButton);
        QString getStyleSheet();

        QApplication& getApplication();
        int getUserId();

    public Q_SLOTS:
        void importMenu(QMenuBar *);
        void switchToLoginPage();
        void switchCredsPage();
        void switchGenPwdPage();
        void disconnect();
        void switchPwdQuality();
        void refreshCredsPage();
        void exportPasswords();
        void importPasswords();
        void switchLeaksPage();
        //void refreshCredsPage();

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
};
#endif