#ifndef _GUI_HEADS_FILE
#define _GUI_HEADS_FILE

#include <QObject>
#include <QApplication>
#include <QPushButton>
#include <QMainWindow>

#define BUTTON_MAX_WIDTH 150
#define BUTTON_MAX_HEIGHT 50

class GuiService : public QObject {
    public:
        GuiService(int argc, char **argv);
        ~GuiService();

        int run();

    private:
        QApplication app;
        QMainWindow mainWindow;
        QPushButton helloButton;
};

#endif