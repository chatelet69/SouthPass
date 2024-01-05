#include <QLineEdit>
#include <QFormLayout>
#include <QApplication>
#include <QMessageBox>
#include "../../includes/applicationController.h"
#include "../../includes/pincludes.h"

loginPage::loginPage(QWidget *parent, MYSQL *dbCon) : QWidget(parent), dbCon(dbCon) {
    //Création des onglets connexion et inscription
    QTabWidget *onglets = new QTabWidget(this);
    QWidget *login = new QWidget;
    QWidget *signIn = new QWidget;

    // Page connexion
    QLabel *loginLabel = new QLabel(this);
    loginLabel->setText("Connexion :");
    QLineEdit *loginEmail = new QLineEdit;
    QLineEdit *loginPassword = new QLineEdit;
    QPushButton *connectButton = new QPushButton("Se connecter");
    QFormLayout *layoutLogin = new QFormLayout;
    // le symbole « & » permet de définir des raccourcis clavier
    layoutLogin->addRow(loginLabel);
    layoutLogin->addRow("&Email :", loginEmail);
    layoutLogin->addRow("&Mot de passe :", loginPassword);
    layoutLogin->addRow(connectButton);
    login->setLayout(layoutLogin);

    // Page inscription
    QLabel *signInLabel = new QLabel(this);
    signInLabel->setText("Inscription :");
    QLineEdit *signEmail = new QLineEdit;
    QLineEdit *signPassword = new QLineEdit;
    QLineEdit *confirmPassword = new QLineEdit;
    QLineEdit *signMasterPwd = new QLineEdit;
    QLineEdit *confirmMasterPwd = new QLineEdit;
    QPushButton *signInButton = new QPushButton("S'inscrire");
    QFormLayout *layoutSignIn = new QFormLayout;
    layoutSignIn->addRow(signInLabel);
    layoutSignIn->addRow("&Email :", signEmail);
    layoutSignIn->addRow("&Mot de passe :", signPassword);
    layoutSignIn->addRow("&Confirmation mot de passe :", confirmPassword);
    layoutSignIn->addRow("Mot de &passe maitre :", signMasterPwd);
    layoutSignIn->addRow("Confirmation mot &de passe :", confirmMasterPwd);
    layoutSignIn->addRow(signInButton);
    signIn->setLayout(layoutSignIn);

    // Ajout des onglets Connexion et Inscription
    onglets->addTab(login, "Connexion");
    onglets->addTab(signIn, "Inscription");

    /* CSS :
    * login->setObjectName("loginTab");
    *login->setObjectName("loginWidget");
    *signIn->setObjectName("signInWidget");
    */

    // Intéraction bouton Inscription
    signInButton->setObjectName("signInButton");
    connect(signInButton, &QPushButton::clicked, [=](){
        // conversion mail de Qtring à char * pour le backend en C
        char signMail[50];
        QByteArray futurStringMail = signEmail->text().toLocal8Bit();
        strcpy(signMail, futurStringMail.data());
        // conversion password de Qtring à char * pour le backend en C
        char password[50];
        QByteArray futurStringPwd = signPassword->text().toLocal8Bit();
        strcpy(password, futurStringPwd.data());

        // conversion verifPassword de Qtring à char * pour le backend en C
        char verifPassword[50];
        QByteArray futurStringConfirmPwd = confirmPassword->text().toLocal8Bit();
        strcpy(verifPassword, futurStringConfirmPwd.data());

        // conversion signMasterPwd de Qtring à char * pour le backend en C
        char masterPwd[50];
        QByteArray futurMasterPwd = signMasterPwd->text().toLocal8Bit();
        strcpy(masterPwd, futurMasterPwd.data());

        // conversion confirmMasterPwd de Qtring à char * pour le backend en C
        char verifMasterPassword[50];
        QByteArray futurMasterConfirmPwd = confirmMasterPwd->text().toLocal8Bit();
        strcpy(verifMasterPassword, futurMasterConfirmPwd.data());

        if(strcmp(verifSignIn(signMail, password, verifPassword,masterPwd, verifMasterPassword), "ok")==0){
            int res;
            res = createUser(dbCon, signMail, password, masterPwd);
            if(res == 0){
                printf("\nInscription réussie !");
                emit signInSuccess();
                return 0;
            }else if(res == 2){
                QMessageBox::warning(this,"Erreur" ,"Email déjà utilisé, essayez de vous connecter");
            }else{
                QMessageBox::warning(this,"Erreur" ,"Erreur lors de la création du compte");
            }
        }else{
            QLabel *signInLabel = new QLabel(this);
            signInLabel->setText(verifSignIn(signMail, password, verifPassword, masterPwd, verifMasterPassword));
            QMessageBox::warning(this,"Erreur" ,verifSignIn(signMail, password, verifPassword, masterPwd, verifMasterPassword));
        }
    });

}


void loginPage::signInSuccess(){
    printf("Succès");
}