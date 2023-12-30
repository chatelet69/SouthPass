#ifndef _GUI_WIDGETS_HEAD_FILE
#define _GUI_WIDGETS_HEAD_FILE

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "models.h"

/*class CredentialsWindow : public QWindow {

};*/

class CredentialsWidget : public QWidget {
    public:
        CredentialsWidget(const CredsArray credsArray, QWidget *parent);

    private:
        QVBoxLayout *layout;
};

class CredsFormWidget : public QWidget {
    public:
        CredsFormWidget(QWidget *parent);

    public Q_SLOTS:
        void closeForm();
};

#endif