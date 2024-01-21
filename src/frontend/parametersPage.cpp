/*
    Filename : parametersPage.cpp
    Description : Parameters Page of Application
*/

#include <QMessageBox>
#include <QRadioButton>
#include <QDesktopServices>
#include "../../includes/parametersPage.hpp"
#include "../../includes/applicationController.hpp"
#include "../../includes/fileController.h"
#include "../../includes/backParams.h"
#include "../../includes/backController.h"
char * convertStringForC(QLineEdit * string);

ParametersPage::ParametersPage(ApplicationController *appController, QApplication *app, MYSQL *dbCon){
    QVBoxLayout * fenetre = new QVBoxLayout(this);

    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    QWidget *mainWidget = new QWidget();
    mainWidget->setObjectName("backParams");
    QVBoxLayout * mainLayout = new QVBoxLayout(mainWidget);

    QLabel *accountTitle = new QLabel("Compte :");
    accountTitle->setObjectName("bigTitle");
    QWidget *emailBox = new QWidget();
    emailBox->setObjectName("paramsBox");
    this->importEmailParameter(emailBox);

    QLabel *secuTitle = new QLabel("Sécurité :");
    secuTitle->setObjectName("bigTitle");
    QWidget *secuBox = new QWidget();
    secuBox->setObjectName("paramsBox");
    this->importSecurityParameter(secuBox);

    QLabel * lookTitle = new QLabel("Apparence :");
    lookTitle->setObjectName("bigTitle");
    QWidget * darkOrLightBox = new QWidget();
    darkOrLightBox->setObjectName("paramsBox");
    QHBoxLayout * darkOrLightLayout = new QHBoxLayout(darkOrLightBox);
    QRadioButton * darkMode = new QRadioButton("Dark mode");
    darkMode->setObjectName("radioBtnParams");
    QRadioButton * lightMode = new QRadioButton("Light Mode");
    darkMode->setObjectName("radioBtnParams");
    if(getThemePreference() == 1){
        darkMode->setChecked(true);
    }else{
        lightMode->setChecked(true);
    }
    darkOrLightLayout->addWidget(darkMode);
    darkOrLightLayout->addWidget(lightMode);

    QLabel * extensionTitle = new QLabel("Extension :");
    extensionTitle->setObjectName("bigTitle");
    QWidget *extensionUrlBox = new QWidget();
    extensionUrlBox->setObjectName("paramsBox");
    QHBoxLayout * extensionUrlLayout = new QHBoxLayout(extensionUrlBox);
    QLabel * extensionLabel = new QLabel("Installer notre extension web :");
    extensionLabel->setObjectName("subTitle");
    QPushButton * extUrlBtn = new QPushButton("Installer");
    extensionUrlLayout->addWidget(extensionLabel);
    extensionUrlLayout->addWidget(extUrlBtn);

    mainLayout->addWidget(accountTitle);
    mainLayout->addWidget(emailBox);

    mainLayout->addWidget(secuTitle);
    mainLayout->addWidget(secuBox);

    mainLayout->addWidget(lookTitle);
    mainLayout->addWidget(darkOrLightBox);

    mainLayout->addWidget(extensionTitle);
    mainLayout->addWidget(extensionUrlBox);

    scrollArea->setWidget(mainWidget);
    fenetre->addWidget(scrollArea);
    setLayout(fenetre);

    connect(extUrlBtn, &QPushButton::clicked, [=](){QDesktopServices::openUrl(QUrl("https://www.youtube.com/watch?v=dQw4w9WgXcQ"));});
    connect(darkMode, &QRadioButton::clicked, [=](){changeThemeMode(appController, app);});
    connect(lightMode, &QRadioButton::clicked, [=](){changeThemeMode(appController, app);});

}

char * ParametersPage::convertStringForC(QLineEdit * string){
    char stringConverted[50];
    QByteArray futurLogPwd = string->text().toLocal8Bit();
    strcpy(stringConverted, futurLogPwd.data());
    if(stringConverted[strlen(stringConverted)-1] == '\n')
        stringConverted[strlen(stringConverted)-1] = '\0';

    return strdup(stringConverted);
}

void ParametersPage::importEmailParameter(QWidget *emailBox) {
    QHBoxLayout *emailLayout = new QHBoxLayout(emailBox);
    TokenInfos *tokenInfos = getTokenFileInfos();
    QVBoxLayout *emailDisplay = new QVBoxLayout();

    QLabel *emailTitle = new QLabel("Email :");
    emailTitle->setObjectName("subTitle");

    QLabel *email = new QLabel();
    if(tokenInfos == NULL || tokenInfos->email == NULL) email->setText("Erreur");
    else email->setText(tokenInfos->email);
    this->userEmail = QString(tokenInfos->email);
    this->userId = tokenInfos->id;

    freeToken(tokenInfos);

    emailDisplay->addWidget(emailTitle);
    emailDisplay->addWidget(email);
    QPushButton *editEmailButton = new QPushButton("Modifier");
    connect(editEmailButton, QPushButton::clicked, this, [this]() { this->showEditEmailBox(this); });
    emailLayout->addLayout(emailDisplay);
    emailLayout->addWidget(editEmailButton);
}

void ParametersPage::importSecurityParameter(QWidget *secuBox) {
    QVBoxLayout * secuLayout = new QVBoxLayout(secuBox);
    QWidget * pwdBox = new QWidget();
    QHBoxLayout * pwdLayout = new QHBoxLayout(pwdBox);

    QLabel * editPwdTitle = new QLabel("Modifier votre mot de passe :");
    editPwdTitle->setObjectName("subTitle");
    QPushButton * editPwdBtn = new QPushButton("Modifier");
    pwdLayout->addWidget(editPwdTitle);
    pwdLayout->addWidget(editPwdBtn);
    secuLayout->addWidget(pwdBox);
    QWidget * pwdBox2 = new QWidget();
    QHBoxLayout * pwdLayout2 = new QHBoxLayout(pwdBox2);
    QLabel * editMasterPwdTitle = new QLabel("Modifier votre mot de passe maitre :");
    editMasterPwdTitle->setObjectName("subTitle");
    QPushButton * editMasterPwdBtn = new QPushButton("Modifier");
    pwdLayout2->addWidget(editMasterPwdTitle);
    pwdLayout2->addWidget(editMasterPwdBtn);
    secuLayout->addWidget(pwdBox2);
}

void ParametersPage::showEditEmailBox(QWidget *parametersParent) {
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QWidget *editEmailWindow = new QWidget(NULL);
    QVBoxLayout *editEmailWindowLayout = new QVBoxLayout(editEmailWindow);
    editEmailWindow->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, QSize(300, 300), primaryScreen->availableGeometry()));
    editEmailWindow->setObjectName("editEmailWindow");

    QLabel *emailLabel = new QLabel(editEmailWindow);
    emailLabel->setText("Email à modifier : ");
    QLineEdit *editEmailInput = new QLineEdit(editEmailWindow);
    editEmailInput->setText(this->userEmail);

    QWidget *buttonsContainer = new QWidget(editEmailWindow);
    QHBoxLayout *buttonsLayout = new QHBoxLayout(buttonsContainer);
    QPushButton *cancelButton = new QPushButton(editEmailWindow);
    cancelButton->setText("Annuler");
    cancelButton->setObjectName("cancelButton");
    QPushButton *editEmailSaveButton = new QPushButton(editEmailWindow);
    editEmailSaveButton->setText("Enregistrer");
    editEmailSaveButton->setObjectName("saveButton");

    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addWidget(editEmailSaveButton);

    connect(cancelButton, &QPushButton::clicked, [=]() {
        editEmailWindow->hide();
        editEmailWindow->close();
    });
    connect(editEmailSaveButton, &QPushButton::clicked, [=]() {
        this->saveNewEmail(editEmailInput->text());
        editEmailWindow->close();
    });

    editEmailWindowLayout->addWidget(emailLabel);
    editEmailWindowLayout->addWidget(editEmailInput);
    editEmailWindowLayout->addWidget(buttonsContainer);

    editEmailWindow->show();
}

void ParametersPage::saveNewEmail(QString emailValue) {
    QByteArray emailBytes = emailValue.toLocal8Bit();
    char *emailConverted = emailBytes.data();
    char *actualEmailConverted = (this->userEmail.toLocal8Bit()).data();
    qDebug() << emailConverted << actualEmailConverted;
    int status = saveEditedEmail(dbCon, this->userId, emailConverted, actualEmailConverted);
    if (status == EXIT_SUCCESS) {
        QMessageBox msgBox;
        QString newMailMessage = QString("Nouveau mail : ").arg(emailValue);
        msgBox.setText("Email mit à jour !");
        msgBox.setInformativeText(newMailMessage);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void changeThemeMode(ApplicationController *appController, QApplication *app){
    if(getThemePreference() == 1){ //  darkMode -> lightMode
        saveThemePreference(0);
        QString styleSheet = appController->getOtherStyleSheet(0);
        app->setStyleSheet(styleSheet);
    }else{ // lightMode -> darkMode
        saveThemePreference(1);
        QString styleSheet = appController->getOtherStyleSheet(1);
        app->setStyleSheet(styleSheet);
    }

}