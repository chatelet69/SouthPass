#ifndef CREDENTIALS_WIDGETS_H
#define CREDENTIALS_WIDGETS_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QObject>
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
        void closeForm(bool refreshStatus);

    signals: 
        void requestRefreshCredsPage();

    private:
        MYSQL *dbCon;
};

class CredsToolBarWidget : public QWidget {
    Q_OBJECT

    public:
        CredsToolBarWidget(QWidget *parent, MYSQL *dbCon);
    
    private:
        MYSQL *dbCon;
        CredsFormWidget *form;

    public Q_SLOTS:

    private Q_SLOTS:
        void showCredsForm();
};

class CredentialEditWidget : public QWidget {
    Q_OBJECT
    public:
        CredentialEditWidget(QWidget *parent, const int credId, QString loginName, QString login, QString password);
        void setCredId(const int newId);
        void setLogin(QString newLogin);
        void setLoginName(QString newName);
        void setPassword(QString newPassword);

    private:
        int credId;
        QString loginName;
        QString login;
        QString password;
};

class CredentialsWidget : public QWidget {
    Q_OBJECT
    public:
        CredentialsWidget(QWidget *parent, const CredsArray credsArray);

    public Q_SLOTS:
        void showEditCred(const int credId, QString name, QString login, QString password);

    private:
        QVBoxLayout *contentLayout;
        QScrollArea *scrollArea;
        CredentialEditWidget *credentialEditWidget;
};

class CredentialsPage : public QWidget {
    Q_OBJECT

    public:
        CredentialsPage(QWidget *parent, MYSQL *dbConnection, int userId);
        //~CredentialsPage();
        void setUserId(int newId);
        void initCredsListWidget();

    public Q_SLOTS:
        //void showCredsForm();

    private Q_SLOTS:
        //void showCredsForm();

    private: 
        MYSQL *dbCon;
        int userId;
        QPushButton newCredsButton;
        CredsToolBarWidget *toolBarWidget;
        CredentialsWidget *credentialsWidget;
};

#endif