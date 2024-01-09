//
// Created by mathf on 08/01/2024.
//
#include <QLineEdit>
#include <QFormLayout>
#include <QApplication>
#include <QCheckBox>
#include <QMessageBox>
#include <QWidget>
#include "../../includes/applicationController.h"
#include "../../includes/pwdQuality.h"
#include "../../includes/pincludes.h"
PwdQualityPage::PwdQualityPage(QWidget *parent, ApplicationController *appController, MYSQL *dbCon) : QWidget(parent), dbCon(dbCon) {
QTabWidget *onglets = new QTabWidget(this);
QWidget *login = new QWidget;
QWidget *signIn = new QWidget;
}