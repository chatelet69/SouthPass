#include <QLineEdit>
#include <QFormLayout>
#include <QApplication>
#include "../../includes/pincludes.h"

loginPage::loginPage(QWidget *parent, MYSQL *dbCon) : QWidget(parent), dbCon(dbCon) {

    QLineEdit *email = new QLineEdit;
    QLineEdit *password = new QLineEdit;
    QFormLayout *layout = new QFormLayout;

    // le symbole « & » permet de définir des raccourcis clavier
    layout->addRow("&Email :", email);
    layout->addRow("&Mot de passe :", password);
    setLayout(layout);
    show();
}
