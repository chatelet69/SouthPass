/*
    Filename : parametersPage.cpp
    Description : Parameters Page of Application (Change email, password, appearance...)
    Last Edit : 21_01_2024
*/

#include <string.h>
#include <QMessageBox>
#include <QRadioButton>
#include <QDesktopServices>
#include "../../includes/parametersPage.hpp"
#include "../../includes/applicationController.hpp"
#include "../../includes/fileController.h"
#include "../../includes/backController.h"
char * convertStringForC(QLineEdit * string);

ParametersPage::ParametersPage(ApplicationController *appController, QApplication *app, MYSQL *dbConnection){
    this->dbCon = dbConnection;
    this->emailCode = 0;
    this->appController = appController;
    QVBoxLayout *paramWindow = new QVBoxLayout(this);

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
    paramWindow->addWidget(scrollArea);
    setLayout(paramWindow);

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
    QVBoxLayout *secuLayout = new QVBoxLayout(secuBox);
    QWidget *pwdBox = new QWidget();
    QHBoxLayout *pwdLayout = new QHBoxLayout(pwdBox);

    QLabel *editPwdTitle = new QLabel("Modifier votre mot de passe :");
    editPwdTitle->setObjectName("subTitle");
    QPushButton *editPwdBtn = new QPushButton("Modifier");
    pwdLayout->addWidget(editPwdTitle);
    pwdLayout->addWidget(editPwdBtn);
    secuLayout->addWidget(pwdBox);

    connect(editPwdBtn, &QPushButton::clicked, this, [this]() {
        this->showEditAccountPassword(QString("pwdAccount"), QString("Mot de passe actuel : "));
    });

    QWidget *pwdBox2 = new QWidget();
    QHBoxLayout *pwdLayout2 = new QHBoxLayout(pwdBox2);
    QLabel *editMasterPwdTitle = new QLabel("Modifier votre mot de passe maitre :");
    editMasterPwdTitle->setObjectName("subTitle");
    QPushButton *editMasterPwdBtn = new QPushButton("Modifier");

    connect(editMasterPwdBtn, &QPushButton::clicked, this, [this]() {
        this->showEditAccountPassword(QString("pwdMaster"), QString("Mot de passe maître actuel : "));
    });

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
        editEmailWindow->close();
        this->saveNewEmail(editEmailInput->text());
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
    char *finalEmail = strdup(emailConverted);
    char *actualEmailFinal = strdup(actualEmailConverted);
    this->showVerifEmailCode(finalEmail, actualEmailFinal);
}

void ParametersPage::showVerifEmailCode(char *newEmail, char *actualEmail) {
    int emailCode = getVerifCode(newEmail);
    if (emailCode != 0) this->importVerifEmailWindow(emailCode, newEmail, actualEmail);
    if (emailCode == 0) QMessageBox::warning(this,"Erreur" ,"Erreur lors de la mise à jour du mail !");
}

void ParametersPage::importVerifEmailWindow(int emailCode, char *email, char *newEmail) {
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QWidget *verifEmailWindow = new QWidget(NULL);
    QVBoxLayout *verifEmailWindowLayout = new QVBoxLayout(verifEmailWindow);
    verifEmailWindow->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, QSize(300, 300), primaryScreen->availableGeometry()));
    verifEmailWindow->setObjectName("verifEmailWindow");

    QLabel *verifLabel = new QLabel(verifEmailWindow);
    verifLabel->setText("Code de vérification :  ");
    QLineEdit *verifCodeInput = new QLineEdit(verifEmailWindow);

    QWidget *buttonsContainer = new QWidget(verifEmailWindow);
    QHBoxLayout *buttonsLayout = new QHBoxLayout(buttonsContainer);
    QPushButton *cancelButton = new QPushButton(verifEmailWindow);
    cancelButton->setText("Annuler");
    cancelButton->setObjectName("cancelButton");
    QPushButton *verifEmailCodeButton = new QPushButton(verifEmailWindow);
    verifEmailCodeButton->setText("Vérifier");
    verifEmailCodeButton->setObjectName("saveButton");

    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addWidget(verifEmailCodeButton);

    connect(cancelButton, &QPushButton::clicked, [=]() {
        verifEmailWindow->hide();
        verifEmailWindow->close();
        verifEmailWindow->deleteLater();
    });

    this->emailCode = emailCode;
    connect(verifEmailCodeButton, &QPushButton::clicked, [=]() {
        this->saveNewEmailFinalStep(verifCodeInput->text(), this->emailCode, email, newEmail);
        verifEmailWindow->close();
        verifEmailWindow->deleteLater();
    });

    verifEmailWindowLayout->addWidget(verifLabel);
    verifEmailWindowLayout->addWidget(verifCodeInput);
    verifEmailWindowLayout->addWidget(buttonsContainer);

    verifEmailWindow->show();
}

void ParametersPage::saveNewEmailFinalStep(QString code, int emailCode, char *newEmail, char *actualEmail) {
    int convertCode = code.toInt();
    int status = -1;
    if (convertCode == emailCode) {
        status = saveEditedEmail(this->dbCon, this->userId, newEmail, actualEmail);
        free(newEmail);
        free(actualEmail);
    }
    if (status == EXIT_SUCCESS) {
        this->showMessageBoxSuccess("Email mit à jour !");
        this->appController->switchParamsPage();
    } else {
        QMessageBox::warning(this,"Erreur" ,"Erreur lors de la mise à jour du mail (ou Code incorrect) !");
    }
}

void ParametersPage::showEditAccountPassword(QString passwordType, QString passwordLabelText) {
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QWidget *editPasswordAccountWindow = new QWidget(NULL);
    QVBoxLayout *editPasswordAccountWindowLayout = new QVBoxLayout(editPasswordAccountWindow);
    editPasswordAccountWindow->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, QSize(300, 300), primaryScreen->availableGeometry()));
    editPasswordAccountWindow->setObjectName("editPasswordAccountWindow");

    QLabel *passwordLabel = new QLabel(editPasswordAccountWindow);
    passwordLabel->setText(passwordLabelText);
    QLineEdit *checkPassInput = new QLineEdit(editPasswordAccountWindow);

    QLabel *newPasswordLabel = new QLabel(editPasswordAccountWindow);
    newPasswordLabel->setText("Nouveau mot de passe : ");
    QLineEdit *newPassInput = new QLineEdit(editPasswordAccountWindow);
    QLabel *newPassConfirmationLabel = new QLabel(editPasswordAccountWindow);
    newPassConfirmationLabel->setText("Confirmation mot de passe : ");
    QLineEdit *newPassConfirmationInput = new QLineEdit(editPasswordAccountWindow);

    QWidget *buttonsContainer = new QWidget(editPasswordAccountWindow);
    QHBoxLayout *buttonsLayout = new QHBoxLayout(buttonsContainer);
    QPushButton *cancelButton = importCancelButton(buttonsContainer, buttonsLayout);
    QPushButton *editPasswordButton = importSaveButton(buttonsContainer, buttonsLayout);

    connect(cancelButton, &QPushButton::clicked, [=]() {
        editPasswordAccountWindow->hide();
        editPasswordAccountWindow->close();
    });
    connect(editPasswordButton, &QPushButton::clicked, [=]() {
        this->saveNewPwd(passwordType, newPassInput->text(), newPassConfirmationInput->text(), checkPassInput->text());
        editPasswordAccountWindow->close();
    });

    editPasswordAccountWindowLayout->addWidget(passwordLabel);
    editPasswordAccountWindowLayout->addWidget(checkPassInput);
    editPasswordAccountWindowLayout->addWidget(newPasswordLabel);
    editPasswordAccountWindowLayout->addWidget(newPassInput);
    editPasswordAccountWindowLayout->addWidget(newPassConfirmationLabel);
    editPasswordAccountWindowLayout->addWidget(newPassConfirmationInput);
    editPasswordAccountWindowLayout->addWidget(buttonsContainer);

    editPasswordAccountWindow->show();
}

void ParametersPage::saveNewPwd(QString type, QString passValue, QString confirmationPass, QString actualPass) {
    QByteArray passBytes = passValue.toLocal8Bit();
    char *passwordConverted = passBytes.data();
    char *actualPassConverted = (actualPass.toLocal8Bit()).data();
    char *passType = (type.toLocal8Bit()).data();
    int status = saveEditedPwdAccount(this->dbCon, this->userId, passwordConverted, actualPassConverted, passType);
    if (status == EXIT_SUCCESS) {
        if (strcmp(passType, "pwdMaster") == 0) this->showMessageBoxSuccess("Mot de passe maître modifié !");
        else this->showMessageBoxSuccess("Mot de passe mit à jour !");
    } else {
        this->showMessageBoxError("Impossible de mettre à jour le mot de passe !", status);
    }
}

QPushButton *ParametersPage::importCancelButton(QWidget *buttonsContainer, QHBoxLayout *buttonsLayout) {
    QPushButton *cancelButton = new QPushButton(buttonsContainer);
    cancelButton->setText("Annuler");
    cancelButton->setObjectName("cancelButton");
    buttonsLayout->addWidget(cancelButton);
    return cancelButton;
}

QPushButton *ParametersPage::importSaveButton(QWidget *buttonsContainer, QHBoxLayout *buttonsLayout) {
    QPushButton *editEmailSaveButton = new QPushButton(buttonsContainer);
    editEmailSaveButton->setText("Enregistrer");
    editEmailSaveButton->setObjectName("saveButton");
    buttonsLayout->addWidget(editEmailSaveButton);
    return editEmailSaveButton;
}

void ParametersPage::showMessageBoxSuccess(QString text) {
    QMessageBox msgBox;
    //QString newMailMessage = QString("Nouveau mail : %1").arg(emailValue);
    msgBox.setText(text);
    //msgBox.setInformativeText(newMailMessage);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void ParametersPage::showMessageBoxError(QString text, int errorType) {
    QMessageBox msgBox;
    QString causeMessage;
    switch (errorType) {
    case -1:
        causeMessage = QString("Cause : Non respect des conditions !");
        break;
    case -2:
        causeMessage = QString("Cause : Mot de passe actuel incorrect !");
        break;
    case -3:
        causeMessage = QString("Cause : Impossible de mettre le même mot de passe que le précédent !");
        break;
    default:
        causeMessage = QString("Erreur indéterminée, Veuillez réessayer plus tard !");
        break;
    }
    msgBox.setText(text);
    msgBox.setInformativeText(causeMessage);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.exec();
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