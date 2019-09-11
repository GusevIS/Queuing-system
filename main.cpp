#include "mainwindow.h"

#include <QApplication>
#include "queuingsystem.h"

int main(int argc, char *argv[])
{
    QueuingSystem queuingSystem;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
