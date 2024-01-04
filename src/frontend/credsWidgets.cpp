/*
    Filename : creds_widget.cpp
    Description : Methods and widget for the creds Main Page
*/

#include <QWidget>
#include <QLabel>
#include <QDir>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QQmlContext>
#include <QQuickWidget>
#include "../../includes/applicationController.h"
#include "../../includes/credentialsWidget.h"
#include "../../includes/backController.h"
#include "../../includes/models.h"

CredentialsWidget::CredentialsWidget(const CredsArray credsArray, QWidget *parent) : QWidget(parent)
{
    contentLayout = new QVBoxLayout(this);
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(300);
    scrollArea->setMaximumHeight(600);

    QWidget *credsContainer = new QWidget(this);
    credsContainer->setObjectName("credsContainer");
    QVBoxLayout *credsLayout = new QVBoxLayout(credsContainer);
    for (unsigned int i = 0; i < credsArray.size; i++) {
        QWidget *credContainer = new QWidget(this);
        credContainer->setObjectName("credBox");
        QHBoxLayout *credContainerLayout = new QHBoxLayout(credContainer);

        QWidget *labelsContainer = new QWidget(credContainer);
        QVBoxLayout *labelsContainerLayout = new QVBoxLayout(labelsContainer);
        
        QLabel *labelName = new QLabel(QString("Nom / Site : %1").arg(credsArray.creds[i].name));
        QLabel *labelLogin = new QLabel(QString("Login : %1").arg(credsArray.creds[i].loginName));
        QLabel *labelPassword = new QLabel(QString("Mot de passe : %1").arg(credsArray.creds[i].password));
        
        labelsContainer->setObjectName("credDetailsBox");
        labelsContainerLayout->addWidget(labelName);
        labelsContainerLayout->addWidget(labelLogin);
        labelsContainerLayout->addWidget(labelPassword);

        QPushButton *button = new QPushButton("Modifier");
        button->setObjectName("editCredButton");

        credContainerLayout->addWidget(labelsContainer);
        credContainerLayout->addWidget(button);
        credsLayout->addWidget(credContainer);
    }

    scrollArea->setWidget(credsContainer);
    contentLayout->addWidget(scrollArea);
    setLayout(contentLayout);
}

CredsFormWidget::CredsFormWidget(QWidget *parent, MYSQL *dbCon) : QWidget(parent), dbCon(dbCon) {
    QQuickWidget *quickWidget = new QQuickWidget();
    quickWidget->setSource(QUrl("./components/credsForm.qml"));

    QQmlContext *context = quickWidget->rootContext();
    context->setContextProperty("CredsFormWidget", this);

    //QPushButton *closeFormButton = new QPushButton("Annuler");
    //connect(closeFormButton, &QPushButton::clicked, this, &CredsFormWidget::closeForm);
    //QLabel *formTitle = new QLabel(QString("Nouveaux identifiants"));
    //this->setWindowTitle("Nouveaux identifiants");

    //int posX = ApplicationController::getApplication().desktop()->availableGeometry().x();
    //this->setGeometry();

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(quickWidget);
    this->setFixedSize(300, 300);
    //layout->addWidget(closeFormButton);
    //layout->addWidget(formTitle);
}

void CredsFormWidget::saveNewCreds(QString name, QString login, QString password) {
    QByteArray nameBytes = name.toLocal8Bit();
    char *convertedName = nameBytes.data();
    QByteArray loginBytes = login.toLocal8Bit();
    char *convertedLogin = loginBytes.data();
    QByteArray passwordBytes = password.toLocal8Bit();
    char *convertedPassword = passwordBytes.data();

    //qDebug() << "name = " << name << " login = " << login << " pass = " << password;
    //qDebug() << "name = " << convertedName << " login = " << convertedLogin << " pass = " << convertedPassword;

    int resStatus = addNewCredsController(dbCon, convertedName, convertedLogin, convertedPassword);
}

void CredsFormWidget::closeForm() {
    this->close();
}