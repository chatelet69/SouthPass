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

CredentialsWidget::~CredentialsWidget() {
    delete credsLayout;
    delete credsContainer;
    delete scrollArea;
    delete contentLayout;
}

void CredentialsWidget::importCredentialsList(CredsArray *credsArray) {
    if (credsArray->size > 0) {
        for (unsigned int i = 0; i < credsArray->size; i++){
            QWidget *credContainer = new QWidget(this);
            credContainer->setObjectName("credBox");
            QHBoxLayout *credContainerLayout = new QHBoxLayout(credContainer);

            QWidget *labelsContainer = new QWidget(credContainer);
            this->importCredDetailsBox(labelsContainer, &credsArray->credentials[i]);

            QWidget *buttonsContainer = new QWidget(this);
            this->importCredButtons(buttonsContainer, labelsContainer, &credsArray->credentials[i]);

            credContainerLayout->addWidget(labelsContainer);
            credContainerLayout->addWidget(buttonsContainer);
            credsLayout->addWidget(credContainer);
        }
    } else {
        QLabel *noPasswordsLabel = new QLabel("Pas de mots de passes");
        noPasswordsLabel->setObjectName("noPasswordsLabel");
        credsLayout->addWidget(noPasswordsLabel);
    }
}

void CredentialsWidget::showEditCred(const int credId, const int userId, QString name, QString login, QString password) {
    CredentialEditWidget *newCredentialEditWidget = new CredentialEditWidget(this, credId, userId, name, login, password);
    newCredentialEditWidget->show();
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

void CredentialsWidget::importCredDetailsBox(QWidget *labelsContainer, Credentials *credentials) {
    if (credentials != NULL && labelsContainer != NULL) {
        QVBoxLayout *labelsContainerLayout = new QVBoxLayout(labelsContainer);

        //QWidget *namesLabelsContainer = new QWidget(this);
        //QVBoxLayout *nameLabelsLayout = new QVBoxLayout(namesLabelsContainer);
        //QLabel *labelNameTitle = new QLabel("Nom");
        //QLabel *labelNameText = new QLabel(credentials->name);
        //nameLabelsLayout->addWidget(labelNameTitle);
        //nameLabelsLayout->addWidget(labelNameText);
        //namesLabelsContainer->setObjectName("nameLabelsContainer");
        QLabel *nameLabel = new QLabel(QString("Nom : %1").arg(credentials->name));
        nameLabel->setObjectName("nameLabel");

        QLabel *loginLabel = new QLabel(QString("Login : %1").arg(credentials->loginName));
        loginLabel->setObjectName("loginLabel");
        QLabel *passwordLabel = new QLabel(QString("Mot de passe : %1").arg(credentials->password));
        passwordLabel->setObjectName("passwordLabel");

        labelsContainer->setObjectName("credDetailsBox");
        labelsContainerLayout->addWidget(nameLabel);
        labelsContainerLayout->addWidget(loginLabel);
        labelsContainerLayout->addWidget(passwordLabel);
    }
}

void CredentialsWidget::importCredButtons(QWidget *buttonsContainer, QWidget *labelsContainer, Credentials *credentials) {
    if (credentials != NULL && buttonsContainer != nullptr && labelsContainer != nullptr) {
        QVBoxLayout *buttonsLayout = new QVBoxLayout(buttonsContainer);
        
        QPushButton *editCredButton = new QPushButton("Modifier");
        editCredButton->setObjectName("editCredButton");
        editCredButton->setProperty("credentialId", credentials->id);
        editCredButton->setProperty("credentialUserId", credentials->userId);
        connect(editCredButton, &QPushButton::clicked, [=](){
            QString tmpName = (labelsContainer->findChild<QLabel*>("nameLabel"))->text();
            QString tmpLoginName = (labelsContainer->findChild<QLabel*>("loginLabel"))->text();
            QString tmpPass = (labelsContainer->findChild<QLabel*>("passwordLabel"))->text();
            int credId = editCredButton->property("credentialId").toInt();
            int userId = editCredButton->property("credentialUserId").toInt();
            this->showEditCred(credId, userId, tmpName, tmpLoginName, tmpPass);
        });

        QPushButton *deleteCredButton = new QPushButton("Supprimer");
        deleteCredButton->setObjectName("deleteCredButton");
        deleteCredButton->setProperty("credentialId", credentials->id);
        deleteCredButton->setProperty("credentialUserId", credentials->userId);
        connect(deleteCredButton, &QPushButton::clicked, [=]() {
            int credId = deleteCredButton->property("credentialId").toInt();
            int userId = deleteCredButton->property("credentialUserId").toInt();
            ((CredentialsPage*)(this->parent()))->deleteCredential(credId, userId);
        });

        buttonsLayout->addWidget(editCredButton);
        buttonsLayout->addWidget(deleteCredButton);
    }
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
    delete form;
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

CredentialEditWidget::CredentialEditWidget(QWidget *widgetParent, const int credId, const int userId, QString name, QString login, QString password) {
    this->credsWidgetParent = widgetParent;
    this->setWindowTitle("Editer l'identifiant");
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, QSize(300, 300), primaryScreen->availableGeometry()));
    mainLayout = new QVBoxLayout(this);

    QWidget *labelsContainer = new QWidget(this);
    QVBoxLayout *labelsContainerLayout = new QVBoxLayout(labelsContainer);
    QWidget *buttonsContainer = new QWidget(this);
    QHBoxLayout *buttonsLayout = new QHBoxLayout(buttonsContainer);

    this->setName(name);
    this->setLogin(login);
    this->setPassword(password);
    this->setCredId(credId);
    this->setUserId(userId);

    this->importEditCredsLabels(labelsContainer, labelsContainerLayout);
    this->importEditCredsButtons(buttonsContainer, buttonsLayout);
}

CredentialEditWidget::~CredentialEditWidget() {
    delete mainLayout;
    delete nameInput;
    delete loginInput;
    delete passwordInput;
    delete cancelEditButton;
    delete saveEditCredButton;
}

void CredentialEditWidget::setCredId(const int newId) { this->credId = newId; }

void CredentialEditWidget::setUserId(const int newUserId) { this->userId = newUserId; }

void CredentialEditWidget::setName(QString newName) { this->name = newName; }

void CredentialEditWidget::setLogin(QString newLogin) { this->login = newLogin; }

void CredentialEditWidget::setPassword(QString newPassword) { this->password = newPassword; }

void CredentialEditWidget::importEditCredsLabels(QWidget *labelsContainer, QVBoxLayout *labelsContainerLayout) {
    if (labelsContainer != nullptr && labelsContainerLayout != nullptr) {
        QLabel *nameLabel = new QLabel("Nom :");
        this->nameInput = new QLineEdit(nameLabel);
        nameInput->insert(this->name.split(": ")[1]); // Pour récupérer seulement le nom (Ex -> Nom : Youtube)

        QLabel *loginLabel = new QLabel("Login :");
        this->loginInput = new QLineEdit(loginLabel);
        loginInput->insert(this->login.split(": ")[1]);

        QLabel *passwordLabel = new QLabel("Mot de passe :");
        this->passwordInput = new QLineEdit(passwordLabel);
        passwordInput->insert(this->password.split(": ")[1]);

        labelsContainer->setObjectName("credEditDetailsBox");
        labelsContainerLayout->addWidget(nameLabel);
        labelsContainerLayout->addWidget(nameInput);
        labelsContainerLayout->addWidget(loginLabel);
        labelsContainerLayout->addWidget(loginInput);
        labelsContainerLayout->addWidget(passwordLabel);
        labelsContainerLayout->addWidget(passwordInput);
        mainLayout->addWidget(labelsContainer);
    }
}

void CredentialEditWidget::importEditCredsButtons(QWidget *buttonsContainer, QHBoxLayout *buttonsLayout) {
    if (buttonsContainer != nullptr && buttonsLayout != nullptr) {        
        this->cancelEditButton = new QPushButton("Annuler");
        this->cancelEditButton->setObjectName("cancelEditButton");
        connect(cancelEditButton, &QPushButton::clicked, [=](){
            this->hide();
            this->close();
        });

        saveEditCredButton = new QPushButton("Enregistrer", this);
        saveEditCredButton->setObjectName("saveEditCredButton");
        saveEditCredButton->setProperty("credentialId", credId);
        this->saveEditCredButton->setProperty("credentialUserId", this->userId);
        connect(saveEditCredButton, &QPushButton::clicked, [=]() {
            int credId = saveEditCredButton->property("credentialId").toInt();
            int userId = saveEditCredButton->property("credentialUserId").toInt();
            QByteArray nameByte = this->nameInput->text().toLocal8Bit();
            QByteArray loginByte = this->loginInput->text().toLocal8Bit();
            QByteArray passByte = this->passwordInput->text().toLocal8Bit();
            const char *convertnameByte = nameByte.constData();
            const char *convertloginByte = loginByte.constData();
            const char *convertpassByte = passByte.constData();
            ((CredentialsPage*)(this->credsWidgetParent->parent()))->saveEditedCreds(credId, userId, 
                                                                        convertnameByte, 
                                                                        convertloginByte,
                                                                        convertpassByte);
            
            this->hide();
            this->close();
        });

        buttonsLayout->addWidget(cancelEditButton);
        buttonsLayout->addWidget(saveEditCredButton);
        mainLayout->addWidget(buttonsContainer);
    }
}