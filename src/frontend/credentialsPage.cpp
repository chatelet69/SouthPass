#include <QDebug>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMainWindow>
#include "../../includes/applicationController.h"
#include "../../includes/db.h"
#include "../../includes/credentialsWidget.h"
#include "../../includes/pincludes.h"

CredentialsPage::CredentialsPage(QWidget *parent, MYSQL *dbConnection, int userId) : QWidget(parent), dbCon(dbCon) {
    this->dbCon = dbConnection;
    this->setUserId(userId);
    QVBoxLayout *layout = new QVBoxLayout();

    toolBarWidget = new CredsToolBarWidget(this, this->dbCon);

    CredsArray credsArray = getPasswordsList(this->dbCon, this->userId);
    qDebug() << "size creds : " << credsArray.size;
    printCreds(credsArray.credentials, credsArray.size);
    this->credentialsWidget = new CredentialsWidget((QWidget *) this, credsArray);
    credentialsWidget->setObjectName("credsWidget");
    qDebug() << "after credsWidget object name";

    layout->addWidget((QWidget *)toolBarWidget);
    layout->addWidget(credentialsWidget);
    setLayout(layout);
    freeCredsArray(&credsArray);
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