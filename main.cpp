#include "mainwindow.h"
#include <QApplication>

MainWindow *w_ptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w_ptr = &w;
    w.show();

    return a.exec();
}
