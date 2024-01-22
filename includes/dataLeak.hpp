#ifndef DATA_LEAK_PAGE_H
#define DATA_LEAK_PAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QObject>
#include <QQmlContext>
#include <QQuickWidget>
#include "models.h"
#include "db.h"

class CredentialsLeaksWidget;
class DataLeaksPage;

class CredentialsLeaksWidget : public QWidget {
    Q_OBJECT

    public:
        CredentialsLeaksWidget(QWidget *parent, MYSQL *dbConnection, int userId);
        //~CredentialsPage();

    public Q_SLOTS:
        //void showCredsForm();

    private Q_SLOTS:
        //void showCredsForm();

    private: 
        MYSQL *dbCon;
        int userId;
};

class DataLeaksPage : public QWidget {
    Q_OBJECT

    public:
        DataLeaksPage(QWidget *parent, MYSQL *dbConnection, int userId);
        //~DataLeaksPage();

    public Q_SLOTS:
        //void showCredsForm();

    private: 
        MYSQL *dbCon;
        int userId;
        CredentialsLeaksWidget *credentialsWidget;
        QVBoxLayout *contentLayout;
        QScrollArea *scrollArea;

        void importLeaksList(LeaksList *leaksList, QVBoxLayout *credsLayout);
};

#endif