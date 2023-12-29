/*
    Filename : qmainwindow.cpp
    Description: Controller of the Qt application
*/

#include <QFile>
#include <QDebug>
#include <QString>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../../includes/db.h"
#include "../../includes/pincludes.h"

#define lightModePath "./src/frontend/css/lightMode.css"
#define darkModePath "./src/frontend/css/darkMode.css"

GuiService::GuiService(int argc,char **argv) : app(argc, argv) {
    isDark = 1;
    dbCon = dbConnect();
    
    mainWindow.setMinimumSize(WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT);
    mainWindow.setWindowTitle("SouthPass");
    mainWindow.setCentralWidget(new QWidget());
    QVBoxLayout *layout = new QVBoxLayout(mainWindow.centralWidget());

    QPushButton *helloButton = new QPushButton();
    helloButton->setText(QPushButton::tr("Hello toi"));
    helloButton->setMaximumSize(BUTTON_MAX_WIDTH, BUTTON_MAX_HEIGHT);
    helloButton->setObjectName("searchButton");
    connect(helloButton, &QPushButton::clicked, this, &GuiService::changeTheme);

    CredsArray credsArray = getPasswordsList(dbCon, 1);
    if (credsArray.size > 0) printCreds(credsArray.creds, credsArray.size);
    credentialsWidget = new CredentialsWidget(credsArray, NULL);
    credentialsWidget->setObjectName("credsWidget");
    freeCredsArray(credsArray);

    QFile file(darkModePath);
    file.open(QFile::ReadOnly | QFile::Text);
    QString styleSheet = QLatin1String(file.readAll());
    styleSheet.remove('\n');
    file.close();

    app.setStyleSheet(styleSheet);
    layout->addWidget(helloButton);
    layout->addWidget(credentialsWidget);
    closeDb(dbCon);
}

GuiService::~GuiService() {
    // Destruction de la classe
}

int GuiService::run() {
    mainWindow.show();
    return app.exec();
}

void GuiService::changeTheme() {
    isDark = (isDark) ? 0 : 1;
    QString themeFile = (isDark) ? darkModePath : lightModePath;

    QFile file(themeFile);
    file.open(QFile::ReadOnly | QFile::Text);
    QString styleSheet = QLatin1String(file.readAll());
    styleSheet.remove('\n');
    file.close();

    app.setStyleSheet(styleSheet);
}