#include <QMainWindow>
#include <QFile>
#include <QString>
#include <QHBoxLayout>
#include "../../includes/gui.h"

GuiService::GuiService(int argc,char **argv) : app(argc, argv) {
    mainWindow.setMinimumSize(400, 400);
    mainWindow.setWindowTitle("SouthPass");

    helloButton.setText(QPushButton::tr("Hello world!"));
    helloButton.setMaximumSize(BUTTON_MAX_WIDTH, BUTTON_MAX_HEIGHT);

    QFile file("./src/gui/style.css");
    file.open(QFile::ReadOnly | QFile::Text);
    QString styleSheet = QLatin1String(file.readAll());
    styleSheet.remove('\n');
    file.close();
    mainWindow.setStyleSheet(styleSheet);

    mainWindow.setCentralWidget(&helloButton);
}

GuiService::~GuiService() {
    // Destruction de la classe
    //delete[] somethign
}

int GuiService::run() {
    mainWindow.show();
    return app.exec();
}