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
    this->setObjectName("dataLeaksPage");
    contentLayout = new QVBoxLayout(this);
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(300);  
    scrollArea->setMaximumHeight(600);

    LeaksList *leaksList = getDataLeaksFromLeakCheck(dbConnection, userId);

    QWidget *credsContainer = new QWidget(this);
    credsContainer->setObjectName("leaksContainer");
    QVBoxLayout *credsLayout = new QVBoxLayout(credsContainer);
    this->importLeaksList(leaksList, credsLayout);

    if (leaksList != NULL) {
        freeLeaksList(leaksList);
        free(leaksList);
    }

    scrollArea->setWidget(credsContainer);
    contentLayout->addWidget(scrollArea);
    setLayout(contentLayout);
}

void DataLeaksPage::importLeaksList(LeaksList *leaksList, QVBoxLayout *credsLayout) {
    if (leaksList != NULL) {
        if (leaksList->count > 0) {
            for (unsigned int i = 0; i < leaksList->count; i++){
                QWidget *credContainer = new QWidget(this);
                credContainer->setObjectName("credLeakBox");
                QHBoxLayout *credContainerLayout = new QHBoxLayout(credContainer);

                QWidget *informationContainer = new QWidget(credContainer);
                QVBoxLayout *informationContainerLayout = new QVBoxLayout(informationContainer);

                QString websiteName = QString(leaksList->credentialLeaks[i].website).remove("\"");
                QString leakDateFormatted = QString(leaksList->credentialLeaks[i].leakDate).remove("\"");
                QLabel *labelWesite = new QLabel(QString("Site source : %1").arg(websiteName));
                QLabel *labelLogin = new QLabel(QString("Mail : %1").arg(leaksList->credentialLeaks[i].login));
                labelLogin->setObjectName("loginLeakedLabel");
                QLabel *labelLeakDate = new QLabel(QString("Date : %1").arg(leakDateFormatted));

                informationContainer->setObjectName("informativeLeakBox");
                informationContainerLayout->addWidget(labelWesite);
                informationContainerLayout->addWidget(labelLeakDate);

                credContainerLayout->addWidget(informationContainer);
                credContainerLayout->addWidget(labelLogin);
                credsLayout->addWidget(credContainer);
            }
        }
    } else {
        QLabel *noPasswordsLabel = new QLabel("Pas de fuites de données (ou Clé invalide)");
        noPasswordsLabel->setObjectName("noDataLeaks");
        credsLayout->addWidget(noPasswordsLabel);
    }
}