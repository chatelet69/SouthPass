/*
    Filename : applicationController.cpp
    Description: Qt Application Conroller
*/

#include <QFile>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
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

    QMenuBar *menuBar = new QMenuBar(nullptr);
    importMenu(menuBar);

    QWidget *headerWidget = new QWidget();
    headerWidget->setObjectName("headerWidget");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);

    QPushButton *themeButton = new QPushButton();
    themeButton->setObjectName("themeButton");
    connect(themeButton, &QPushButton::clicked, [=]() { this->changeTheme(themeButton); });
    QIcon icon("./assets/lightThemeIcon.png");
    themeButton->setIcon(icon);

    headerLayout->addWidget(menuBar, 0, Qt::AlignLeft);
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

void ApplicationController::importMenu(QMenuBar *menuBar){
// MENU
    QMenu *menuFichier = menuBar->addMenu("Fichiers");
    QAction *importPwd = new QAction("Importer des mots de passes", this);
    menuFichier->addAction(importPwd);
    QAction *exportPwd = new QAction("Exporter des mots de passes", this);
    menuFichier->addAction(exportPwd);

    QMenu *menuOutils = menuBar->addMenu("Outils");
    QAction *seePwd = new QAction("Voir mes mots de passes", this);
    menuOutils->addAction(seePwd);
    QAction *pwdGenerator = new QAction("Générateur de mot passes", this);
    menuOutils->addAction(pwdGenerator);
    QAction *pwdQuality = new QAction("Qualité des mots de passes", this);
    menuOutils->addAction(pwdQuality);
    QAction *analysis = new QAction("Analyse des fuites de données", this);
    menuOutils->addAction(analysis);

    QMenu *menuSouthPass = menuBar->addMenu("SouthPass");
    QAction *deco = new QAction("Se déconnecter", this);
    menuSouthPass->addAction(deco);
    QAction *quitWindow = new QAction("Quitter SouthPass", this);
    menuSouthPass->addAction(quitWindow);

    connect(seePwd, SIGNAL(triggered()), this, SLOT(switchCredsPage()));
    connect(quitWindow, SIGNAL(triggered()), qApp, SLOT(quit()));
}