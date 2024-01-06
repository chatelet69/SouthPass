/*
    Filename : applicationController.cpp
    Description: Qt Application Conroller
*/

#include <QFile>
#include <QDebug>
#include <QString>
#include <QPushButton>
#include <QtWidgets>
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
    
    QWidget *mainWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainWidget->setLayout(mainLayout);

    QString styleSheet = ApplicationController::getStyleSheet();
    app.setStyleSheet(styleSheet);

    QWidget *headerWidget = new QWidget();
    headerWidget->setObjectName("headerWidget");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);

    QPushButton *themeButton = new QPushButton();
    themeButton->setObjectName("themeButton");
    connect(themeButton, &QPushButton::clicked, [=]() {
        this->changeTheme(themeButton);
        qDebug() << "x : " << mainWindow.getXPos();
    });

    QIcon icon("./assets/lightThemeIcon.png");
    themeButton->setIcon(icon);

    headerLayout->addWidget(themeButton, 0, Qt::AlignRight);

    mainWindow.move(500, 500);
    qDebug() << "x : " << mainWindow.getXPos();

    logPage = new loginPage(NULL,this, dbCon);
    credsPage = new CredentialsPage(NULL, dbCon);
    stackedWidget->addWidget(credsPage);
    stackedWidget->addWidget(logPage);

    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(stackedWidget);
    mainWindow.setCentralWidget(mainWidget);

    if(isConnected() == 0){
        ApplicationController::switchCredsPage();
    }else{
        ApplicationController::switchToLoginPage();
        // connect(logPage, reinterpret_cast<const char *>(&loginPage::signInSuccess), this, SLOT(switchCredsPage()));
    }
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
void ApplicationController::switchToLoginPage() {
    stackedWidget->setCurrentWidget(logPage);
}