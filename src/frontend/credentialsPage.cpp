#include <QDebug>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMainWindow>
#include "../../includes/applicationController.hpp"
#include "../../includes/db.h"
#include "../../includes/credentialsWidget.hpp"
#include "../../includes/pincludes.h"

CredentialsPage::CredentialsPage(QWidget *parent, MYSQL *dbConnection, int userId) : QWidget(parent), dbCon(dbCon) {
    this->dbCon = dbConnection;
    this->setUserId(userId);
    QVBoxLayout *layout = new QVBoxLayout();

    toolBarWidget = new CredsToolBarWidget(this, this->dbCon);

    CredsArray *credsArray = getPasswordsList(this->dbCon, this->userId);
    credentialsWidget = new CredentialsWidget((QWidget *) this, credsArray);
    credentialsWidget->setObjectName("credsWidget");
    freeCredsArray(credsArray);
    free(credsArray);

    layout->addWidget((QWidget *)toolBarWidget);
    layout->addWidget(credentialsWidget);
    setLayout(layout);
}

void CredentialsPage::setUserId(int newId) {
    this->userId = newId;
}

void CredentialsPage::initCredsListWidget() {
    //CredsArray credsArray = getPasswordsList(this->dbCon, this->userId);
    //this->credentialsWidget = new CredentialsWidget(this, credsArray);
    //credentialsWidget->setObjectName("credsWidget");
    //freeCredsArray(credsArray);
}

void CredentialsPage::showAllCredentials() {
    CredsArray *credsArray = getPasswordsList(this->dbCon, this->userId);

    if (credsArray != NULL) {
        if (credsArray->size > 0) this->credentialsWidget->updateCredentialsList(credsArray);
        freeCredsArray(credsArray);
        free(credsArray);
    }
}

void CredentialsPage::refreshSearchCreds(char *searchedValue) {
    if (searchedValue != NULL) {
        CredsArray *credsArray = searchCredsBy(this->dbCon, this->userId, searchedValue, "loginName");

        if (credsArray != NULL) {
            if (credsArray->size > 0) this->credentialsWidget->updateCredentialsList(credsArray);
            freeCredsArray(credsArray);
            free(credsArray);
        }

    }
}