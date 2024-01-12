/*
    Filename : creds_widget.cpp
    Description : Methods and widget for the creds Main Page
*/

#include <QWidget>
#include <QLabel>
#include <QDir>
#include <QObject>
#include <QQuickStyle>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QLineEdit>
#include <QQmlContext>
#include <QComboBox>
#include <QQuickWidget>
#include "../../includes/credentialsWidget.h"
#include "../../includes/applicationController.h"
#include "../../includes/backController.h"
#include "../../includes/models.h"

CredentialsWidget::CredentialsWidget(QWidget *parent, const CredsArray credsArray) : QWidget(parent)
{
    contentLayout = new QVBoxLayout(this);
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(300);
    scrollArea->setMaximumHeight(600);

    QWidget *credsContainer = new QWidget(this);
    credsContainer->setObjectName("credsContainer");
    QVBoxLayout *credsLayout = new QVBoxLayout(credsContainer);
    qDebug() << "credsContainer : " << credsArray.size << " : " << credsArray.credentials;
    if (credsArray.size > 0) {
        qDebug() << "size is not 0 : " << credsArray.size;
        for (unsigned int i = 0; i < credsArray.size; i++){
            QWidget *credContainer = new QWidget(this);
            credContainer->setObjectName("credBox");
            QHBoxLayout *credContainerLayout = new QHBoxLayout(credContainer);

            QWidget *labelsContainer = new QWidget(credContainer);
            QVBoxLayout *labelsContainerLayout = new QVBoxLayout(labelsContainer);

            QLabel *labelName = new QLabel(QString("Nom : %1").arg(credsArray.credentials[i].name));
            QLabel *labelLogin = new QLabel(QString("Login : %1").arg(credsArray.credentials[i].loginName));
            QLabel *labelPassword = new QLabel(QString("Mot de passe : %1").arg(credsArray.credentials[i].password));

            labelsContainer->setObjectName("credDetailsBox");
            labelsContainerLayout->addWidget(labelName);
            labelsContainerLayout->addWidget(labelLogin);
            labelsContainerLayout->addWidget(labelPassword);

            QPushButton *button = new QPushButton("Modifier");
            button->setObjectName("editCredButton");
            connect(button, &QPushButton::clicked, [=](){
                        QString tmpName = QString::fromUtf8(credsArray.credentials[i].loginName, -1);
                        qDebug() << "tmpName : " << tmpName;
                        // QString tmpLogin = new QString("%1").arg(credsArray.creds[i].loginName);
                        // QString tmpPass = new QString("%1").arg(credsArray.creds[i].password);
                        // showEditCred(credsArray.creds[i].id, tmpName, tmpLogin, tmpPass);
                    });

            credContainerLayout->addWidget(labelsContainer);
            credContainerLayout->addWidget(button);
            credsLayout->addWidget(credContainer);
        }
    }

    scrollArea->setWidget(credsContainer);
    contentLayout->addWidget(scrollArea);
    setLayout(contentLayout);
}

void CredentialsWidget::showEditCred(const int credId, QString name, QString login, QString password) {
    qDebug() << "name: " << name << " : " << login << " : " <<  password;
    this->credentialEditWidget = new CredentialEditWidget(this, credId, name, login, password);
    this->credentialEditWidget->setCredId(credId);
    this->credentialEditWidget->setLoginName(name);
    this->credentialEditWidget->setLogin(login);
    this->credentialEditWidget->setPassword(password);
    this->credentialEditWidget->show();
}

// CredsForm

CredsFormWidget::CredsFormWidget(QWidget *parent, MYSQL *dbCon) : QWidget(parent), dbCon(dbCon) {
    QQuickStyle::setStyle("Fusion");
    quickWidget = new QQuickWidget();
    quickWidget->setSource(QUrl("./components/credsForm.qml"));

    QQmlContext *context = quickWidget->rootContext();
    context->setContextProperty("CredsFormWidget", this);

    quickWidget->setFixedSize(320,320);
    quickWidget->setWindowTitle("Nouveaux identifiants");
    quickWidget->setObjectName("credsFormWindow");
}

void CredsFormWidget::saveNewCreds(QString name, QString login, QString password) {
    QByteArray nameBytes = name.toLocal8Bit();
    char *convertedName = nameBytes.data();
    QByteArray loginBytes = login.toLocal8Bit();
    char *convertedLogin = loginBytes.data();
    QByteArray passwordBytes = password.toLocal8Bit();
    char *convertedPassword = passwordBytes.data();

    int resStatus = addNewCredsController(dbCon, convertedName, convertedLogin, convertedPassword);
    this->closeForm(true);
}

void CredsFormWidget::closeForm(bool refreshStatus) {
    this->quickWidget->close();
    this->close();
    if (refreshStatus) {
        qDebug() << "refresh";
        Q_EMIT requestRefreshCredsPage();
    }
}

// ToolBar

CredsToolBarWidget::CredsToolBarWidget(QWidget *parent, MYSQL *dbCon) : QWidget(parent), dbCon(dbCon)
{
    QHBoxLayout *toolBarLayout = new QHBoxLayout(this);

    form = new CredsFormWidget(this, dbCon);
    QPushButton *newCredsButton = new QPushButton(this);
    newCredsButton->setText(QPushButton::tr("Nouvel identifiant"));
    newCredsButton->setMaximumSize(BUTTON_MAX_WIDTH, BUTTON_MAX_HEIGHT);
    newCredsButton->setObjectName("newCredsButton");
    connect(newCredsButton, &QPushButton::clicked, this, &CredsToolBarWidget::showCredsForm);

    QLineEdit *searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Chercher un id");
    searchInput->setObjectName("searchCredsInput");

    QComboBox *searchType = new QComboBox();
    searchType->setPlaceholderText("Type");
    searchType->setObjectName("credsSearchTypeBox");
    searchType->insertItem(1, "Site");
    searchType->insertItem(0, "Mail");

    toolBarLayout->addWidget(searchInput);
    toolBarLayout->addWidget(searchType);
    toolBarLayout->addWidget(newCredsButton);
}

void CredsToolBarWidget::showCredsForm() {
    form->quickWidget->show();
}

// Cred Edit Window

CredentialEditWidget::CredentialEditWidget(QWidget *parent, const int credId, QString loginName, QString login, QString password) {
    this->setWindowTitle("Editer l'identifiant");

    QWidget *labelsContainer = new QWidget(this);
    QVBoxLayout *labelsContainerLayout = new QVBoxLayout(labelsContainer);

    QLineEdit *nameInput = new QLineEdit(this);
    QLabel *labelName = new QLabel("Nom :");
    nameInput->insert(loginName);
    nameInput->setParent(labelName);
    //labelName->setBuddy(nameInput);

    QLabel *labelLogin = new QLabel("Login :");
    QLabel *labelPassword = new QLabel("Mot de passe :");

    labelsContainer->setObjectName("credDetailsBox");
    labelsContainerLayout->addWidget(labelName);
    labelsContainerLayout->addWidget(labelLogin);
    labelsContainerLayout->addWidget(labelPassword);
}

void CredentialEditWidget::setCredId(const int newId) {
    this->credId = newId;
}

void CredentialEditWidget::setLoginName(QString newName) {
    this->loginName = newName;
}

void CredentialEditWidget::setLogin(QString newLogin) {
    this->login = newLogin;
}

void CredentialEditWidget::setPassword(QString newPassword) {
    this->password = newPassword;
}