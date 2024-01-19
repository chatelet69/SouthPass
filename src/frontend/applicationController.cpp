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

    // Tâche synchrone
    QTimer::singleShot(0, this, [=]() {
        logPage = new LoginPage(stackedWidget, this, dbCon);
        credsPage = new CredentialsPage(stackedWidget, dbCon, this->userId);
        pwdGen = new PwdGenerator(stackedWidget, this, dbCon);
        pwdQual = new PwdQualityPage(stackedWidget, this, dbCon);
        dataLeaksPage = new DataLeaksPage(stackedWidget, dbCon, this->userId);

        stackedWidget->addWidget(credsPage);
        stackedWidget->addWidget(logPage);
        stackedWidget->addWidget(pwdGen);
        stackedWidget->addWidget(pwdQual);
        stackedWidget->addWidget(dataLeaksPage);
    });

    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(stackedWidget);
    mainWindow.setCentralWidget(mainWidget);

    if(isConnected() == 0 && userId != 0){
        ApplicationController::switchCredsPage();
    } else {
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

    QPushButton *themeButton = new QPushButton();
    themeButton->setObjectName("themeButton");
    connect(themeButton, &QPushButton::clicked, [=]() { this->changeTheme(themeButton); });

    const char *themeIconPath = (isDark) ? lightModeIcon : darkModeIcon;
    QIcon icon(themeIconPath);
    themeButton->setIcon(icon);

    headerLayout->addWidget(menuBar, 0, Qt::AlignLeft);
    //headerLayout->addWidget(menuBar);
    headerLayout->addWidget(themeButton, 0, Qt::AlignRight);
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

void ApplicationController::switchCredsPage() {
    if(isConnected() == 0 && this->credsPage != NULL) {
        this->credsPage->showAllCredentials();
        stackedWidget->setCurrentWidget(credsPage);
    }
}

void ApplicationController::switchGenPwdPage() {
    if(isConnected() == 0)
        stackedWidget->setCurrentWidget(pwdGen);
}

void ApplicationController::switchPwdQuality() {
    if(isConnected() == 0)
        stackedWidget->setCurrentWidget(pwdQual);
}

QApplication& ApplicationController::getApplication() {
    return app;
}

void ApplicationController::switchToLoginPage() {
    stackedWidget->setCurrentWidget(logPage);
}

void ApplicationController::switchLeaksPage() {
    stackedWidget->setCurrentWidget(dataLeaksPage);
}

int ApplicationController::getUserId() {
    return userId;
}

void ApplicationController::importMenu(QMenuBar *menuBar){
// MENU
    QMenu *menuFichier = menuBar->addMenu("Fichiers");
    menuFichier->setObjectName("fileMenu");
    QAction *importPwd = new QAction("Importer des mots de passes", this);
    menuFichier->addAction(importPwd);
    QAction *exportPwd = new QAction("Exporter des mots de passes", this);
    menuFichier->addAction(exportPwd);
    
    connect(importPwd, &QAction::triggered, this, &ApplicationController::importPasswords);
    connect(exportPwd, &QAction::triggered, this, &ApplicationController::exportPasswords);

    QMenu *menuOutils = menuBar->addMenu("Outils");
    menuOutils->setObjectName("toolsMenu");
    QAction *seePwd = new QAction("Voir mes mots de passes", this);
    menuOutils->addAction(seePwd);
    QAction *pwdGenerator = new QAction("Générateur de mot passes", this);
    menuOutils->addAction(pwdGenerator);
    QAction *pwdQuality = new QAction("Qualité des mots de passes", this);
    menuOutils->addAction(pwdQuality);
    QAction *analysis = new QAction("Analyse des fuites de données", this);
    menuOutils->addAction(analysis);
    connect(analysis, &QAction::triggered, this, &ApplicationController::switchLeaksPage);

    QMenu *menuSouthPass = menuBar->addMenu("SouthPass");
    QAction *deco = new QAction("Se déconnecter", this);
    menuSouthPass->addAction(deco);
    QAction *quitWindow = new QAction("Quitter SouthPass", this);
    menuSouthPass->addAction(quitWindow);
    connect(seePwd, SIGNAL(triggered()), this, SLOT(switchCredsPage()));
    connect(pwdGenerator, SIGNAL(triggered()), this, SLOT(switchGenPwdPage()));
    connect(pwdQuality, SIGNAL(triggered()), this, SLOT(switchPwdQuality()));
    connect(deco, SIGNAL(triggered()), this, SLOT(disconnect()));
    connect(quitWindow, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void ApplicationController::disconnect() {
    createTokenFile();
    switchToLoginPage();
}

void ApplicationController::importPasswords() {
    QString importedFile = QFileDialog::getOpenFileName(NULL, "Fichier CSV à importer", QDir::homePath(), tr("Csv files (*.csv)"));
    QByteArray importedFileBytes = importedFile.toLocal8Bit();
    char *importedPasswordsFile = importedFileBytes.data();
    int status(importPasswordsController(dbCon, userId, importedPasswordsFile));
    if (status == EXIT_SUCCESS) {
        QMessageBox::warning(stackedWidget,"Succès" ,"Mots de passes importés avec succès !");
    } else {
        QMessageBox::warning(stackedWidget,"Erreur" ,"Erreur lors de l'importation des mots de passes !");
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