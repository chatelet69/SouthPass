#ifndef _GUI_WIDGETS_HEAD_FILE
#define _GUI_WIDGETS_HEAD_FILE

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "./models.h"
#include "./db.h"

class CredsFormWidget : public QWidget {
    public:
        CredsFormWidget(QWidget *parent);

    public Q_SLOTS:
        void closeForm();
};

class CredentialsWidget : public QWidget {
    public:
        CredentialsWidget(const CredsArray credsArray, QWidget *parent);

    private:
        QVBoxLayout *layout;
};

class CredentialsPage : public QWidget {
    public:
        CredentialsPage(QWidget *parent, MYSQL *dbCon);
        //~CredentialsPage();

    public Q_SLOTS:
        void showPage();
        void showCredsForm();

    private Q_SLOTS:
        //void showCredsForm();

    private: 
        MYSQL *dbCon;
        QPushButton newCredsButton;
        CredsFormWidget *form;
        CredentialsWidget *credentialsWidget;
};

#endif