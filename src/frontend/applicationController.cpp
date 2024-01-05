/*
    Filename : applicationController.cpp
    Description: Qt Application Conroller
*/

#include <QFile>
#include <QDebug>
#include <QString>
#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>
#include <QIcon>
#include <QHBoxLayout>
#include "../../includes/models.h"
#include "../../includes/db.h"
#include "../../includes/pincludes.h"
#define lightModePath "./style/lightMode.css"
#define darkModePath "./style/darkMode.css"

ApplicationController::ApplicationController(int argc,char **argv) : app(argc, argv) {
    isDark = 1;
    dbCon = dbConnect();
    stackedWidget = new QStackedWidget(NULL);
    
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QString styleSheet = ApplicationController::getStyleSheet();
    app.setStyleSheet(styleSheet);

    QPushButton *themeButton = new QPushButton();
    //themeButton->setText(QPushButton::tr("Mettre Light Mode"));
    //themeButton->setMaximumSize(BUTTON_MAX_WIDTH, BUTTON_MAX_HEIGHT);
    themeButton->setObjectName("themeButton");
    connect(themeButton, &QPushButton::clicked, [=]() {
        this->changeTheme(themeButton);
        qDebug() << "x : " << mainWindow.getXPos();
    });

    QIcon icon("./assets/lightThemeIcon.png");
    themeButton->setIcon(icon);

    mainWindow.move(500, 500);
    qDebug() << "x : " << mainWindow.getXPos();

    credsPage = new CredentialsPage(NULL, dbCon);
    stackedWidget->addWidget(credsPage);

    mainLayout->addWidget(themeButton);
    mainLayout->addWidget(stackedWidget);
    QWidget *mainWidget = new QWidget();
    mainWidget->setLayout(mainLayout);
    mainWindow.setCentralWidget(mainWidget);

    ApplicationController::switchCredsPage();
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

    const char *themeIconPath = (isDark) ? "./assets/lightThemeIcon.png" : "./assets/darkThemeIcon.png";

    QIcon icon(themeIconPath);
    themeButton->setIcon(icon);

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

QApplication& ApplicationController::getApplication() {
    return app;
}