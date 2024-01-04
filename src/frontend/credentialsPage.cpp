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

CredentialsPage::CredentialsPage(QWidget *parent, MYSQL *dbCon) : QWidget(parent), dbCon(dbCon) {
    QVBoxLayout *layout = new QVBoxLayout();

    form = new CredsFormWidget(NULL, dbCon);
    QPushButton *newCredsButton = new QPushButton();
    newCredsButton->setText(QPushButton::tr("Nouveaux creds"));
    newCredsButton->setMaximumSize(BUTTON_MAX_WIDTH, BUTTON_MAX_HEIGHT);
    connect(newCredsButton, &QPushButton::clicked, this, &CredentialsPage::showCredsForm);

    CredsArray credsArray = getPasswordsList(dbCon, 1);
    if (credsArray.size > 0) printCreds(credsArray.creds, credsArray.size);
    credentialsWidget = new CredentialsWidget(credsArray, NULL);
    credentialsWidget->setObjectName("credsWidget");
    freeCredsArray(credsArray);

    layout->addWidget(newCredsButton);
    layout->addWidget(credentialsWidget);
    setLayout(layout);
}

void CredentialsPage::showPage() {
    dynamic_cast<ApplicationController*>(parentWidget())->switchCredsPage();
}

void CredentialsPage::showCredsForm() {
    form->show();
}