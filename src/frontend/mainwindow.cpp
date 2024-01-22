/*
    Filename : mainwindow.cpp
    Description : MainWindow Class (Represent the Main Window of the application)
*/

#include <QScreen>
#include <QMenu>
#include <QMenuBar>
#include <QStyle>
#include "../../includes/applicationController.hpp"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), appController(appController) 
{
    setMinimumSize(WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT);
    setWindowTitle(APPLICATION_NAME);

    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QRect screenDimensions = primaryScreen->availableGeometry();
    
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), primaryScreen->availableGeometry()));
}