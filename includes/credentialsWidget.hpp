#ifndef CREDENTIALS_WIDGETS_H
#define CREDENTIALS_WIDGETS_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QObject>
#include <QLineEdit>
#include <QComboBox>
#include <QQmlContext>
#include <QQuickWidget>
#include "./models.h"
#include "./db.h"

class CredsFormWidget : public QWidget {
    Q_OBJECT

    public:
        CredsFormWidget(QWidget *parent, MYSQL* dbCon);
        QQuickWidget *quickWidget;

    public Q_SLOTS:
        void saveNewCreds(QString loginName, QString login, QString password);
        void closeNewCredsForm(bool refreshStatus);

    signals: 
        void requestRefreshCredsPage();

    private:
        MYSQL *dbCon;
};

class CredsToolBarWidget : public QWidget {
    Q_OBJECT

    public:
        CredsToolBarWidget(QWidget *parent, MYSQL *dbCon);
        ~CredsToolBarWidget();
    
    private:
        MYSQL *dbCon;
        CredsFormWidget *form;
        QComboBox *searchType;
        QLineEdit *searchInput;

    public Q_SLOTS:
        void searchCreds();
        void showCredsForm();
};

class CredentialEditWidget : public QWidget {
    Q_OBJECT
    public:
        CredentialEditWidget(QWidget *widgetParent, const int credId, const int userId,
         QString loginName, QString login, QString password);
        ~CredentialEditWidget();
        void setCredId(const int newId);
        void setUserId(const int newUserId);
        void setLogin(QString newLogin);
        void setName(QString newName);
        void setPassword(QString newPassword);

    private:
        QWidget *credsWidgetParent;
        QVBoxLayout *mainLayout;
        QLineEdit *nameInput;
        QLineEdit *loginInput;
        QLineEdit *passwordInput;
        QPushButton *cancelEditButton;
        QPushButton *saveEditCredButton;

        int credId;
        int userId;
        QString name;
        QString login;
        QString password;

        void importEditCredsLabels(QWidget *labelsContainer, QVBoxLayout *labelsContainerLayout);
        void importEditCredsButtons(QWidget *buttonsContainer, QHBoxLayout *buttonsLayout);
};

class CredentialsWidget : public QWidget {
    Q_OBJECT
    public:
        CredentialsWidget(QWidget *parent, CredsArray *credsArray);
        void updateCredentialsList(CredsArray *credsArray);
        void importCredentialsList(CredsArray *credsArray);

    public Q_SLOTS:
        void showEditCred(const int credId, const int userId, QString name, QString login, QString password);

    private:
        QVBoxLayout *contentLayout;
        QScrollArea *scrollArea;
        CredentialEditWidget *credentialEditWidget;
        QWidget *credsContainer;
        QVBoxLayout *credsLayout;

        void importCredDetailsBox(QWidget *labelsContainer, Credentials *credentials);
        void importCredButtons(QWidget *buttonsContainer, QWidget *labelsContainer, Credentials *credentials);
};

class CredentialsPage : public QWidget {
    Q_OBJECT

    public:
        CredentialsPage(QWidget *parent, MYSQL *dbConnection, int userId);
        ~CredentialsPage();
        void setUserId(int newId);
        void initCredsListWidget();

    public Q_SLOTS:
        void refreshSearchCreds(char *searchedValue, int typeValue);
        void showAllCredentials();
        void deleteCredential(int credentialId, int userId);
        void saveEditedCreds(int credId, int userId, const char *name, const char *login, const char *password);
        //void showCredsForm();

    private: 
        MYSQL *dbCon;
        int userId;
        QPushButton newCredsButton;
        CredsToolBarWidget *toolBarWidget;
        CredentialsWidget *credentialsWidget;
};

#endif