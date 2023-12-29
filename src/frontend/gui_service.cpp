/*
    Filename : gui_service.cpp
*/

#include <QFile>
#include <QDebug>
#include <QString>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../../includes/db.h"
#include "../../includes/pincludes.h"

GuiService::GuiService(int argc,char **argv) : app(argc, argv) {
    mainWindow.setMinimumSize(WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT);
    mainWindow.setWindowTitle("SouthPass");
    mainWindow.setCentralWidget(new QWidget());
    QVBoxLayout *layout = new QVBoxLayout(mainWindow.centralWidget());

    QPushButton *helloButton = new QPushButton();
    helloButton->setText(QPushButton::tr("Hello toi"));
    helloButton->setMaximumSize(BUTTON_MAX_WIDTH, BUTTON_MAX_HEIGHT);
    helloButton->setObjectName("searchButton");

    MYSQL *dbCon = dbConnect();
    CredsArray credsArray = getPasswordsList(dbCon, 1);
    if (credsArray.size > 0) printCreds(credsArray.creds, credsArray.size);
    credentialsWidget = new CredentialsWidget(credsArray, NULL);
    credentialsWidget->setObjectName("credsWidget");
    freeCredsArray(credsArray);

    QFile file("./src/frontend/darkMode.css");
    file.open(QFile::ReadOnly | QFile::Text);
    QString styleSheet = QLatin1String(file.readAll());
    styleSheet.remove('\n');
    file.close();
    //mainWindow.setStyleSheet(styleSheet);
    app.setStyleSheet(styleSheet);

    layout->addWidget(helloButton);
    layout->addWidget(credentialsWidget);
    closeDb(dbCon);
}

GuiService::~GuiService() {
    // Destruction de la classe
    // delete something
}

int GuiService::run() {
    mainWindow.show();
    return app.exec();
}