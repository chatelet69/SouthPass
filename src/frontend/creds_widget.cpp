/*
    Filename : creds_widget.cpp
    Description : Methods and widget for the creds Main Page
*/

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include "../../includes/gui_widgets.h"
#include "../../includes/models.h"

CredentialsWidget::CredentialsWidget(const CredsArray credsArray, QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout(this);

    for (unsigned int i = 0; i < credsArray.size; i++) {
        QWidget *container = new QWidget(this);
        container->setObjectName("credBox");
        
        QWidget *labelsContainer = new QWidget(container);
        QVBoxLayout *labelsLayout = new QVBoxLayout(labelsContainer);
        QHBoxLayout *containerLayout = new QHBoxLayout(container);
        
        QLabel *labelName = new QLabel(QString("Nom / Site : %1").arg(credsArray.creds[i].name));
        QLabel *labelLogin = new QLabel(QString("Login : %1").arg(credsArray.creds[i].loginName));
        QLabel *labelPassword = new QLabel(QString("Mot de passe : %1").arg(credsArray.creds[i].password));
        
        labelsContainer->setObjectName("credDetailsBox");
        labelsLayout->addWidget(labelName);
        labelsLayout->addWidget(labelLogin);
        labelsLayout->addWidget(labelPassword);

        QPushButton *button = new QPushButton("Modifier");
        button->setObjectName("editCredButton");

        containerLayout->addWidget(labelsContainer);
        containerLayout->addWidget(button);
        layout->addWidget(container);
    }

    setLayout(layout);
}

CredsFormWidget::CredsFormWidget(QWidget *parent) : QWidget(parent) {
    QPushButton *closeFormButton = new QPushButton("Annuler");
    connect(closeFormButton, &QPushButton::clicked, this, &CredsFormWidget::closeForm);

    QLabel *formTitle = new QLabel(QString("Nouveaux identifiants"));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(closeFormButton);
    layout->addWidget(formTitle);
}

void CredsFormWidget::closeForm() {
    this->close();
}