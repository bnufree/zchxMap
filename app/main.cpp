#include "testmainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestMainWindow w;
    w.showMaximized();
    return a.exec();
}
