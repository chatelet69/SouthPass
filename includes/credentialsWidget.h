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
        void closeForm();

    private:
        MYSQL *dbCon;
};

class CredentialsWidget : public QWidget {
    public:
        CredentialsWidget(const CredsArray credsArray, QWidget *parent);

    private:
        QVBoxLayout *contentLayout;
        QScrollArea *scrollArea;
};

class CredentialsPage : public QWidget {
    Q_OBJECT

    public:
        CredentialsPage(QWidget *parent, MYSQL *dbCon);
        //~CredentialsPage();

    public Q_SLOTS:
        //void showCredsForm();

    private Q_SLOTS:
        void showCredsForm();

    private: 
        MYSQL *dbCon;
        QPushButton newCredsButton;
        CredsFormWidget *form;
        CredentialsWidget *credentialsWidget;
};

#endif