/*
    Filename : credentialsPage.cpp
    Description : Main page, containing the list of credentials
    Last Edit : 21_01_2024
*/

#include <QDebug>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QMainWindow>
#include "../../includes/applicationController.hpp"
#include "../../includes/db.h"
#include "../../includes/credentialsWidget.hpp"
#include "../../includes/pincludes.h"

CredentialsPage::CredentialsPage(QWidget *parent, MYSQL *dbConnection, int userId) : QWidget(parent), dbCon(dbCon) {
    this->dbCon = dbConnection;
    this->userId = userId;
    pageLayout = new QVBoxLayout();

    toolBarWidget = new CredsToolBarWidget(this, this->dbCon);

    CredsArray *credsArray = getPasswordsList(this->dbCon, this->userId);
    credentialsWidget = new CredentialsWidget((QWidget *) this, credsArray);
    credentialsWidget->setObjectName("credsWidget");
    freeCredsArray(credsArray);
    free(credsArray);

    pageLayout->addWidget(toolBarWidget);
    pageLayout->addWidget(credentialsWidget);
    setLayout(pageLayout);
}

CredentialsPage::~CredentialsPage() {
    delete toolBarWidget;
    delete credentialsWidget;
    delete pageLayout;
}

void CredentialsPage::setUserId(int newId) {
    this->userId = newId;
}

void CredentialsPage::showAllCredentials() {
    CredsArray *credsArray = getPasswordsList(this->dbCon, this->userId);

    if (credsArray != NULL) {
        this->credentialsWidget->updateCredentialsList(credsArray);
        freeCredsArray(credsArray);
        free(credsArray);
    }
}

void CredentialsPage::refreshSearchCreds(char *searchedValue, int typeValue) {
    if (searchedValue != NULL) {
        const char *type = (typeValue) ? "name" : "loginName";
        CredsArray *credsArray = searchCredsBy(this->dbCon, this->userId, searchedValue, type);

        if (credsArray != NULL) {
            if (credsArray->size > 0) this->credentialsWidget->updateCredentialsList(credsArray);
            freeCredsArray(credsArray);
            free(credsArray);
        }

    }
}

void CredentialsPage::deleteCredential(int credentialId, int userId) {
    QMessageBox msgBox;
    msgBox.setText("Cela va supprimer l'identifiant");
    msgBox.setInformativeText("Etes-vous sûr ?");
    msgBox.setIcon(QMessageBox::Question);
    QPushButton *confirmButton = msgBox.addButton(tr("Confirmer"), QMessageBox::ActionRole);
    QPushButton *abortButton = msgBox.addButton(QMessageBox::Abort);
    abortButton->setText("Annuler");
    int ret = msgBox.exec();
    if (msgBox.clickedButton() == confirmButton) {
        int status = deleteCredentialController(this->dbCon, credentialId, userId);
        if (status == EXIT_SUCCESS) {
            this->showAllCredentials();
        }
    }
}

void CredentialsPage::saveEditedCreds(int credId, int userId, const char *name, const char *login, const char *password) {
    int status = saveEditedCredsController(this->dbCon, credId, userId, name, login, password);
    if (status == EXIT_SUCCESS) {
        QMessageBox msgBox;
        msgBox.setText("Identifiant mit à jour !");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
        this->showAllCredentials();
    } else {
        QMessageBox::warning(this, "Erreur" ,"Erreur lors de la mise à jour de l'identifiant !");
    }
}