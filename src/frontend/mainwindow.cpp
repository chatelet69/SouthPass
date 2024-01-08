/*
    Filename : mainwindow.cpp
    Description : MainWindow Class (Represent the Main Window of the application)
*/

#include <QDebug>
#include <QScreen>
#include <QMenu>
#include <QMenuBar>
#include <QStyle>
#include "../../includes/applicationController.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), appController(appController) 
{
    setMinimumSize(WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT);
    setWindowTitle(APPLICATION_NAME);

    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QRect screenDimensions = primaryScreen->availableGeometry();

    //int x = screenDimensions.width() / 2;
    //int y = screenDimensions.height() / 2;
    //qDebug() << "x : " << x << " y : " << y;

    //move(x, y);
    //setGeometry(x, y, WINDOW_MIN_WIDTH+50, WINDOW_MIN_HEIGHT+50);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), primaryScreen->availableGeometry()));
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