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

    toolBarWidget = new CredsToolBarWidget(this, dbCon);

    CredsArray credsArray = getPasswordsList(dbCon, 1);
    credentialsWidget = new CredentialsWidget(this, credsArray);
    credentialsWidget->setObjectName("credsWidget");
    freeCredsArray(credsArray);

    layout->addWidget(toolBarWidget);
    layout->addWidget(credentialsWidget);
    setLayout(layout);
}