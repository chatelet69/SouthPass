#ifndef _GUI_HEADS_FILE
#define _GUI_HEADS_FILE

#include <QObject>
#include <QApplication>
#include <QMainWindow>
#include <QStackedWidget>
#include "./db.h"
#include "./gui_widgets.h"

#define applicationName "SouthPass"

#define WINDOW_MIN_WIDTH 475
#define WINDOW_MIN_HEIGHT 400

#define BUTTON_MAX_WIDTH 150
#define BUTTON_MAX_HEIGHT 50

class ApplicationController : public QObject {
    public:
        ApplicationController(int argc, char **argv);
        ~ApplicationController();

        int run();
        void changeTheme(QPushButton *themeButton);
        QString getStyleSheet();

    public Q_SLOTS:
        void switchCredsPage();

    private:
        char isDark;
        QApplication app;
        QStackedWidget *stackedWidget;
        QMainWindow mainWindow;
        MYSQL *dbCon;
        CredentialsPage *credsPage;
};

#endif