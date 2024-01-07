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

#define APPLICATION_NAME "SouthPass"

#define WINDOW_MIN_WIDTH 500
#define WINDOW_MIN_HEIGHT 500

#define BUTTON_MAX_WIDTH 150
#define BUTTON_MAX_HEIGHT 50

class MainWindow;
class ApplicationController;
class loginPage;

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

    public Q_SLOTS:
        void importMenu(QMenuBar *);
        void switchCredsPage();
        void switchToLoginPage();
        // void onApplicationMove();

    public slots:
        void onSeePwdClicked();


private:
        char isDark;
        QApplication app;
        QStackedWidget *stackedWidget;
        MainWindow mainWindow;
        MYSQL *dbCon;
        loginPage *logPage;
        CredentialsPage *credsPage;
};
#endif