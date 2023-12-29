#ifndef _GUI_HEADS_FILE
#define _GUI_HEADS_FILE

#include <QObject>
#include <QApplication>
#include <QMainWindow>
#include "./gui_widgets.h"

#define WINDOW_MIN_WIDTH 400
#define WINDOW_MIN_HEIGHT 400

#define BUTTON_MAX_WIDTH 150
#define BUTTON_MAX_HEIGHT 50

class GuiService : public QObject {
    public:
        GuiService(int argc, char **argv);
        ~GuiService();

        int run();
        void changeTheme();

    private:
        char isDark;
        QApplication app;
        QMainWindow mainWindow;
        MYSQL *dbCon;
        CredentialsWidget *credentialsWidget;
};

#endif