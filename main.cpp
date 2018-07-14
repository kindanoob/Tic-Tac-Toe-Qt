#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(kWindowWidthInPx, kWindowHeightInPx);
    w.setWindowTitle(kWindowTitle);
    w.show();
    return a.exec();
}
