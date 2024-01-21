/*
    Filename : dataLeakPage.cpp
    Description : Page that displays possible data leaks linked to emails
    Last Edit : 20_01_2024
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
#include <QQuickWidget>
#include "../../includes/dataLeak.hpp"
#include "../../includes/applicationController.hpp"
#include "../../includes/backController.h"

DataLeaksPage::DataLeaksPage(QWidget *parent, MYSQL *dbConnection, int userId) : QWidget(parent)
{
    contentLayout = new QVBoxLayout(this);
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(300);  
    scrollArea->setMaximumHeight(600);

    //getDataLeaks(dbCon, userId);
    //getRemainingCreditsIntelx();
    LeaksList *leaksList = getDataLeaksFromLeakCheck(dbConnection, userId);

    QWidget *credsContainer = new QWidget(this);
    credsContainer->setObjectName("credsContainer");
    QVBoxLayout *credsLayout = new QVBoxLayout(credsContainer);
    if (leaksList != NULL) {
        if (leaksList->count > 0) {
            for (unsigned int i = 0; i < leaksList->count; i++){
                QWidget *credContainer = new QWidget(this);
                credContainer->setObjectName("credLeakBox");
                QHBoxLayout *credContainerLayout = new QHBoxLayout(credContainer);

                QWidget *labelsContainer = new QWidget(credContainer);
                QVBoxLayout *labelsContainerLayout = new QVBoxLayout(labelsContainer);

                QLabel *labelWesite = new QLabel(QString("Site : %1").arg(leaksList->credentialLeaks[i].website));
                QLabel *labelLogin = new QLabel(QString("Mail : %1").arg(leaksList->credentialLeaks[i].login));
                QLabel *labelLeakDate = new QLabel(QString("Date : %1").arg(leaksList->credentialLeaks[i].leakDate));

                labelsContainer->setObjectName("credLeakDetailBox");
                labelsContainerLayout->addWidget(labelWesite);
                labelsContainerLayout->addWidget(labelLogin);
                labelsContainerLayout->addWidget(labelLeakDate);

                credContainerLayout->addWidget(labelsContainer);
                credsLayout->addWidget(credContainer);
            }
        }
    } else {
        QLabel *noPasswordsLabel = new QLabel("Pas de fuites de données (ou Clé invalide)");
        noPasswordsLabel->setObjectName("noDataLeaks");
        credsLayout->addWidget(noPasswordsLabel);
    }

    if (leaksList != NULL) {
        freeLeaksList(leaksList);
        free(leaksList);
    }

    scrollArea->setWidget(credsContainer);
    contentLayout->addWidget(scrollArea);
    setLayout(contentLayout);
}