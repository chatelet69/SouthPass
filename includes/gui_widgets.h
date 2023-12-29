#ifndef _GUI_WIDGETS_HEAD_FILE
#define _GUI_WIDGETS_HEAD_FILE

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "models.h"

class CredentialsWidget : public QWidget {
    public:
        CredentialsWidget(const CredsArray credsArray, QWidget *parent);

    private:
        QVBoxLayout *layout;
};

#endif