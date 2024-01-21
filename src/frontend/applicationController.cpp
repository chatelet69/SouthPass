/*
    Filename : applicationController.cpp
    Description: Qt / SouthPass Application Controller
*/

#include <QFile>
#include <QMenu>
#include <QMainWindow>
#include <QAction>
#include <QMenuBar>
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <QPushButton>
#include <QFuture>
#include <QtWidgets>
#include <QVBoxLayout>
#include <QApplication>
#include <QIcon>
#include <QHBoxLayout>
#include "../../includes/models.h"
#include "../../includes/db.h"
#include "../../includes/applicationController.hpp"
#include "../../includes/pincludes.h"
#include "../../includes/pwdQuality.h"
#include "../../includes/fileController.h"
#include "../../includes/backLoginSignIn.h"
#include "../../includes/pwdGeneratorPage.h"
#include "../../includes/parametersPage.hpp"

ApplicationController::ApplicationController(int argc,char **argv) : /*QObject(nullptr),*/ app(argc, argv) {
    isDark = getThemePreference();
    oldTheme = isDark;
    dbCon = dbConnect();
    stackedWidget = new QStackedWidget(NULL);
    userId = getUserIdByToken(dbCon);

    QWidget *mainWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainWidget->setLayout(mainLayout);
    stackedWidget->setParent(mainWidget);

    QString styleSheet = this->getStyleSheet();
    app.setStyleSheet(styleSheet);
    //app.setStyle(QStyleFactory::create("Windows"));

    QWidget *headerWidget = new QWidget();
    this->importHeader(headerWidget);
    // Tâche synchroneQTimer::singleShot(0, this, [=]() {
    pwdGen = new PwdGenerator(stackedWidget, this, dbCon);
    stackedWidget->addWidget(pwdGen);
    //});

    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(stackedWidget);
    mainWindow.setCentralWidget(mainWidget);

    if(isConnected() == 0 && userId != 0){
        ApplicationController::switchCredsPage();
    } else {
        printf("\nswitchToLoginPage");
        ApplicationController::switchToLoginPage();
    }
}

void ApplicationController::loadAsyncPages() {
    /*pwdGen = new PwdGenerator(stackedWidget, this, dbCon);
    pwdQual = new PwdQualityPage(stackedWidget, this, dbCon);
    dataLeaksPage = new DataLeaksPage(stackedWidget, dbCon, this->userId);

    stackedWidget->addWidget(pwdGen);
    stackedWidget->addWidget(pwdQual);
    stackedWidget->addWidget(dataLeaksPage);*/
}

ApplicationController::~ApplicationController() {
    // Destruction de la classe
    closeDb(dbCon);

    if (oldTheme != isDark) saveThemePreference(isDark);

    delete credsPage;
    delete pwdQual;
    delete logPage;
}

int ApplicationController::run() {
    mainWindow.show();
    return app.exec();
}

void ApplicationController::importHeader(QWidget *headerWidget) {
    QMenuBar *menuBar = new QMenuBar(nullptr);
    //menuBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    importMenu(menuBar);

    headerWidget->setObjectName("headerWidget");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
/*
    QPushButton *themeButton = new QPushButton();
    themeButton->setObjectName("themeButton");
    connect(themeButton, &QPushButton::clicked, [=]() { this->changeTheme(themeButton); });

    const char *themeIconPath = (isDark) ? lightModeIcon : darkModeIcon;
    QIcon icon(themeIconPath);
    themeButton->setIcon(icon);
*/
    headerLayout->addWidget(menuBar, 0, Qt::AlignLeft);
    //headerLayout->addWidget(menuBar);
    //headerLayout->addWidget(themeButton, 0, Qt::AlignRight);
}

void ApplicationController::changeTheme(QPushButton *themeButton) {
    isDark = (isDark) ? 0 : 1;
    QString themeFile = (isDark) ? darkModePath : lightModePath;

    const char *themeIconPath = (isDark) ? lightModeIcon : darkModeIcon;

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
    const char *path = (isDark) ? darkModePath : lightModePath;
    QFile file(path);
    file.open(QFile::ReadOnly | QFile::Text);
    QString styleSheet = QLatin1String(file.readAll());
    styleSheet.remove('\n');
    file.close();
    return styleSheet;
}

QString ApplicationController::getOtherStyleSheet(int darkOrNot) {
    const char *path = darkOrNot ? darkModePath : lightModePath;
    QFile file(path);
    file.open(QFile::ReadOnly | QFile::Text);
    QString styleSheet = QLatin1String(file.readAll());
    styleSheet.remove('\n');
    file.close();
    return styleSheet;
}

void ApplicationController::switchCredsPage() {
    qDebug() << "Stacked : " << stackedWidget->children();
    if (credsPage) {
        stackedWidget->removeWidget(credsPage);
        qDebug() << "before delete";
        qDebug() << credsPage;
        delete credsPage;
        qDebug() << "after : " << credsPage;
        //this->credsPage = nullptr;
    }
    qDebug() << credsPage;
    credsPage = new CredentialsPage(stackedWidget, dbCon, this->userId);
    stackedWidget->addWidget(credsPage);
    if(isConnected() == 0 && credsPage != NULL) {
        credsPage->showAllCredentials();
        stackedWidget->setCurrentWidget(credsPage);
    }
}

void ApplicationController::switchGenPwdPage() {
    if(isConnected() == 0)
        stackedWidget->setCurrentWidget(pwdGen);
}

void ApplicationController::switchPwdQuality() {
    pwdQual = new PwdQualityPage(stackedWidget, this, dbCon);
    stackedWidget->addWidget(pwdQual);
    if(isConnected() == 0)
        stackedWidget->setCurrentWidget(pwdQual);
}

QApplication& ApplicationController::getApplication() {
    return app;
}

void ApplicationController::switchToLoginPage() {
    logPage = new LoginPage(stackedWidget, this, dbCon);
    stackedWidget->addWidget(logPage);
    stackedWidget->setCurrentWidget(logPage);
}

void ApplicationController::switchLeaksPage() {
    dataLeaksPage = new DataLeaksPage(stackedWidget, dbCon, this->userId);
    stackedWidget->addWidget(dataLeaksPage);
    if(isConnected() == 0)
        stackedWidget->setCurrentWidget(dataLeaksPage);
}

void ApplicationController::switchParamsPage() {
    paramsPage = new ParametersPage(this, &app, dbCon);
    stackedWidget->addWidget(paramsPage);
    if(isConnected() == 0)
        stackedWidget->setCurrentWidget(paramsPage);
}

int ApplicationController::getUserId() {
    return userId;
}

void ApplicationController::importMenu(QMenuBar *menuBar){
// MENU
    QMenu *fileMenu = menuBar->addMenu("Fichiers");
    fileMenu->setObjectName("fileMenu");
    QAction *importPwd = new QAction("Importer des mots de passes", this);
    fileMenu->addAction(importPwd);
    QAction *exportPwd = new QAction("Exporter des mots de passes", this);
    fileMenu->addAction(exportPwd);
    
    connect(importPwd, &QAction::triggered, this, &ApplicationController::importPasswords);
    connect(exportPwd, &QAction::triggered, this, &ApplicationController::exportPasswords);

    QMenu *toolsMenu = menuBar->addMenu("Outils");
    toolsMenu->setObjectName("toolsMenu");
    QAction *seePwd = new QAction("Voir mes mots de passes", this);
    toolsMenu->addAction(seePwd);
    QAction *pwdGenerator = new QAction("Générateur de mot passes", this);
    toolsMenu->addAction(pwdGenerator);
    QAction *pwdQuality = new QAction("Qualité des mots de passes", this);
    toolsMenu->addAction(pwdQuality);
    QAction *analysis = new QAction("Analyse des fuites de données", this);
    toolsMenu->addAction(analysis);
    connect(analysis, &QAction::triggered, this, &ApplicationController::switchLeaksPage);

    QMenu *menuSouthPass = menuBar->addMenu("SouthPass");
    QAction *paramsBtn = new QAction("Paramètres", this);
    menuSouthPass->addAction(paramsBtn);
    QAction *deco = new QAction("Se déconnecter", this);
    menuSouthPass->addAction(deco);
    QAction *quitWindow = new QAction("Quitter SouthPass", this);
    menuSouthPass->addAction(quitWindow);
    connect(seePwd, SIGNAL(triggered()), this, SLOT(switchCredsPage()));
    connect(pwdGenerator, SIGNAL(triggered()), this, SLOT(switchGenPwdPage()));
    connect(pwdQuality, SIGNAL(triggered()), this, SLOT(switchPwdQuality()));
    connect(paramsBtn, SIGNAL(triggered()), this, SLOT(switchParamsPage()));
    connect(deco, SIGNAL(triggered()), this, SLOT(disconnect()));
    connect(quitWindow, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void ApplicationController::disconnect() {
    createTokenFile();
    switchToLoginPage();
}

void ApplicationController::importPasswords() {
    QString importedFile = QFileDialog::getOpenFileName(NULL, "Fichier CSV à importer", QDir::homePath(), tr("Csv files (*.csv)"));
    if (importedFile.size() > 0) {
        QByteArray importedFileBytes = importedFile.toLocal8Bit();
        char *importedPasswordsFile = importedFileBytes.data();
        int status(importPasswordsController(dbCon, userId, importedPasswordsFile));
        if (status == EXIT_SUCCESS) {
            QMessageBox::warning(stackedWidget,"Succès" ,"Mots de passes importés avec succès !");
        } else {
            QMessageBox::warning(stackedWidget,"Erreur" ,"Erreur lors de l'importation des mots de passes !");
        }
    }
}

void ApplicationController::exportPasswords() {
    QString exportFolder = QFileDialog::getExistingDirectory(NULL, "Dossier ou exporter", QDir::homePath());
    if (exportFolder.size() > 0) {
        QByteArray exportFolderBytes = exportFolder.toLocal8Bit();
        char *exportFolderConverted = exportFolderBytes.data();
        int status(exportPasswordsController(dbCon, userId, exportFolderConverted));
        if (status == EXIT_SUCCESS) {
            QMessageBox msgBox;
            QString exportedFolder = QString("Chemin de l'export : %1").arg(exportFolder);
            msgBox.setInformativeText(exportedFolder);
            msgBox.setText("Mots de passes exportés!");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
        } else {
            QMessageBox::warning(stackedWidget,"Erreur" ,"Erreur lors de l'exportation des mots de passes !");
        }
    }
}

void ApplicationController::refreshCredsPage() {
    qDebug() << "Test";
    if (stackedWidget) {
        // Utiliser stackedWidget en toute sécurité ici
        qDebug() << "ok";
    } else {
        qDebug() << "stackedWidget is nullptr!";
    }
    for (int i = 0; i < stackedWidget->count(); ++i) {
        QWidget *widget = stackedWidget->widget(i);
        qDebug() << "Widget at index" << i << ":" << widget;
    }
    int oldPageIndex = stackedWidget->indexOf(credsPage);
    qDebug() << "old index : " << oldPageIndex;
    stackedWidget->removeWidget(credsPage);
    //delete credsPage;
    CredentialsPage *newCredsPage = new CredentialsPage(NULL, dbCon, userId);
    credsPage = newCredsPage;
    stackedWidget->insertWidget(oldPageIndex, credsPage);
}

void ApplicationController::deleteChildsOfLayout(QLayout *layout) {
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        // On parcourt la liste d'enfants pour les supprimer
        delete child->widget();
        delete child;
    }
}