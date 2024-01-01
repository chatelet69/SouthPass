/*
    Filename : qmainwindow.cpp
    Description: Qt Application Conroller
*/

#include <QFile>
#include <QDebug>
#include <QString>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../../includes/models.h"
#include "../../includes/db.h"
#include "../../includes/pincludes.h"
#define lightModePath "./src/frontend/css/lightMode.css"
#define darkModePath "./src/frontend/css/darkMode.css"

ApplicationController::ApplicationController(int argc,char **argv) : app(argc, argv) {
    isDark = 1;
    dbCon = dbConnect();
    stackedWidget = new QStackedWidget(NULL);
    
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainWindow.setMinimumSize(WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT);
    mainWindow.setWindowTitle(applicationName);

    QString styleSheet = ApplicationController::getStyleSheet();
    app.setStyleSheet(styleSheet);

    QPushButton *themeButton = new QPushButton();
    themeButton->setText(QPushButton::tr("Mettre Light Mode"));
    themeButton->setMaximumSize(BUTTON_MAX_WIDTH, BUTTON_MAX_HEIGHT);
    themeButton->setObjectName("searchButton");
    connect(themeButton, &QPushButton::clicked, [=]() {
        this->changeTheme(themeButton);
    });

    credsPage = new CredentialsPage(NULL, dbCon);
    stackedWidget->addWidget(credsPage);

    mainLayout->addWidget(themeButton);
    mainLayout->addWidget(stackedWidget);
    QWidget *mainWidget = new QWidget();
    mainWidget->setLayout(mainLayout);
    mainWindow.setCentralWidget(mainWidget);

    ApplicationController::switchCredsPage();

    //closeDb(dbCon);
}

ApplicationController::~ApplicationController() {
    // Destruction de la classe
    closeDb(dbCon);
}

int ApplicationController::run() {
    mainWindow.show();
    return app.exec();
}

void ApplicationController::changeTheme(QPushButton *themeButton) {
    isDark = (isDark) ? 0 : 1;
    QString themeFile = (isDark) ? darkModePath : lightModePath;

    const char *themeName = (isDark) ? "Mettre LightMode" : "Mettre DarkMode";
    themeButton->setText(QPushButton::tr(themeName));

    QFile file(themeFile);
    file.open(QFile::ReadOnly | QFile::Text);
    QString styleSheet = QLatin1String(file.readAll());
    styleSheet.remove('\n');
    file.close();

    app.setStyleSheet(styleSheet);
}

QString ApplicationController::getStyleSheet() {
    QFile file(darkModePath);
    file.open(QFile::ReadOnly | QFile::Text);
    QString styleSheet = QLatin1String(file.readAll());
    styleSheet.remove('\n');
    file.close();
    return styleSheet;
}

void ApplicationController::switchCredsPage() {
    stackedWidget->setCurrentWidget(credsPage);
}