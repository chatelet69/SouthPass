/*
    Filename : mainwindow.cpp
    Description : MainWindow Class (Represent the Main Window of the application)
*/

#include <QDebug>
#include <QScreen>
#include "../../includes/applicationController.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), appController(appController) 
{
    setMinimumSize(WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT);
    setFixedSize(WINDOW_MIN_WIDTH+50, WINDOW_MIN_HEIGHT+50);
    setWindowTitle(APPLICATION_NAME);

    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QRect screenDimensions = primaryScreen->availableGeometry();

    int x = screenDimensions.width() / 2;
    int y = screenDimensions.height() / 2;

    move(x, y);
    setGeometry(x, y, WINDOW_MIN_WIDTH+50, WINDOW_MIN_HEIGHT+50);

    //connect(this, &MainWindow::move, this, &MainWindow::onApplicationMove);
}

void MainWindow::onApplicationMove(QMoveEvent *event) {
    qDebug() << " coordonnées : x =" << this->x() << ", y =" << this->y();
    QMainWindow::moveEvent(event);
}

int MainWindow::getXPos() {
    qDebug() << "X : " << this->geometry();
    return this->x();
}