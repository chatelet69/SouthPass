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
#include "../../includes/credentialsWidget.hpp"
#include "../../includes/applicationController.hpp"
#include "../../includes/backController.h"
#include "../../includes/models.h"

CredentialsWidget::CredentialsWidget(QWidget *parent, CredsArray *credsArray) : QWidget(parent)
{
    contentLayout = new QVBoxLayout(this);
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(300);
    scrollArea->setMaximumHeight(600);

    credsContainer = new QWidget(this);
    credsContainer->setObjectName("credsContainer");
    credsLayout = new QVBoxLayout(credsContainer);
    if (credsArray->size > 0) {
        this->importCredentialsList(credsArray);
    } else {
        QLabel *noPasswordsLabel = new QLabel("Pas de mots de passes");
        noPasswordsLabel->setObjectName("noPasswordsLabel");
        credsLayout->addWidget(noPasswordsLabel);
    }

    scrollArea->setWidget(credsContainer);
    contentLayout->addWidget(scrollArea);
    setLayout(contentLayout);
    this->show();
}

void CredentialsWidget::importCredentialsList(CredsArray *credsArray) {
    for (unsigned int i = 0; i < credsArray->size; i++){
        QWidget *credContainer = new QWidget(this);
        credContainer->setObjectName("credBox");
        QHBoxLayout *credContainerLayout = new QHBoxLayout(credContainer);
            
        QWidget *labelsContainer = new QWidget(credContainer);
        QVBoxLayout *labelsContainerLayout = new QVBoxLayout(labelsContainer);

        QLabel *labelName = new QLabel(QString("Nom : %1").arg(credsArray->credentials[i].name));
        QLabel *labelLogin = new QLabel(QString("Login : %1").arg(credsArray->credentials[i].loginName));
        QLabel *labelPassword = new QLabel(QString("Mot de passe : %1").arg(credsArray->credentials[i].password));

        labelsContainer->setObjectName("credDetailsBox");
        labelsContainerLayout->addWidget(labelName);
        labelsContainerLayout->addWidget(labelLogin);
        labelsContainerLayout->addWidget(labelPassword);

        QWidget *buttonsContainer = new QWidget(this);
        QVBoxLayout *buttonsLayout = new QVBoxLayout(buttonsContainer);
        QPushButton *editCredButton = new QPushButton("Modifier");
        editCredButton->setObjectName("editCredButton");
        connect(editCredButton, &QPushButton::clicked, [=](){
            //QString tmpName = QString::fromUtf8(credsArray->credentials[i].loginName, -1);
            //qDebug() << "tmpName : " << tmpName;
            // QString tmpLogin = new QString("%1").arg(credsArray.creds[i].loginName);
            // QString tmpPass = new QString("%1").arg(credsArray.creds[i].password);
            // showEditCred(credsArray.creds[i].id, tmpName, tmpLogin, tmpPass);
        });

        QPushButton *deleteCredButton = new QPushButton("Supprimer");
        deleteCredButton->setObjectName("deleteCredButton");
        deleteCredButton->setProperty("credentialId", credsArray->credentials[i].id);
        deleteCredButton->setProperty("credentialUserId", credsArray->credentials[i].userId);
        connect(deleteCredButton, &QPushButton::clicked, [=]() {
            int credId = deleteCredButton->property("credentialId").toInt();
            int userId = deleteCredButton->property("credentialUserId").toInt();
            ((CredentialsPage*)(this->parent()))->deleteCredential(credId, userId);
        });

        buttonsLayout->addWidget(editCredButton);
        buttonsLayout->addWidget(deleteCredButton);

        credContainerLayout->addWidget(labelsContainer);
        credContainerLayout->addWidget(buttonsContainer);
        credsLayout->addWidget(credContainer);
    }
}

void CredentialsWidget::showEditCred(const int credId, QString name, QString login, QString password) {
    this->credentialEditWidget = new CredentialEditWidget(this, credId, name, login, password);
    this->credentialEditWidget->setCredId(credId);
    this->credentialEditWidget->setLoginName(name);
    this->credentialEditWidget->setLogin(login);
    this->credentialEditWidget->setPassword(password);
    this->credentialEditWidget->show();
}

void CredentialsWidget::updateCredentialsList(CredsArray *credsArray) {
    if (credsArray != NULL) {
        // Delete de l'ancien contenu et ajout de la nouvelle liste
        ApplicationController *tmpApp;
        dynamic_cast<ApplicationController*>(tmpApp)->deleteChildsOfLayout(credsLayout);
        this->importCredentialsList(credsArray);
    }
    this->repaint();
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
    this->closeNewCredsForm(true);
}

void CredsFormWidget::closeNewCredsForm(bool refreshStatus) {
    this->quickWidget->close();
    this->close();
    if (refreshStatus) {
        ((CredentialsPage*)parent()->parent())->showAllCredentials();
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

    searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Chercher un identifiant");
    searchInput->setObjectName("searchCredsInput");
    //connect(searchInput, &QLineEdit::textChanged, this, &CredsToolBarWidget::searchCreds);

    QPushButton *searchCredsButton = new QPushButton(this);
    searchCredsButton->setText(QPushButton::tr("Chercher"));
    searchCredsButton->setObjectName("searchCredsButton");
    connect(searchCredsButton, &QPushButton::clicked, this, &CredsToolBarWidget::searchCreds);

    searchType = new QComboBox();
    searchType->setObjectName("credsSearchTypeBox");
    searchType->insertItem(0, "Email");
    searchType->insertItem(1, "Site");
    searchType->setCurrentIndex(0);

    toolBarLayout->addWidget(searchInput);
    toolBarLayout->addWidget(searchCredsButton);
    toolBarLayout->addWidget(searchType);
    toolBarLayout->addWidget(newCredsButton);
}

CredsToolBarWidget::~CredsToolBarWidget() {
    delete searchType;
    delete searchInput;
}

void CredsToolBarWidget::showCredsForm() {
    form->quickWidget->show();
}

void CredsToolBarWidget::searchCreds() {
    QString inputValue = this->searchInput->text();
    int typeValue = this->searchType->currentIndex();
    if ((int) inputValue.size() > 0) {
        QByteArray inputByteArray = inputValue.toUtf8();
        ((CredentialsPage*)parentWidget())->refreshSearchCreds(inputByteArray.data(), typeValue);
    } else {
        ((CredentialsPage*)parentWidget())->showAllCredentials();
    }
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