#include "mainwindow.h"
#include "extractdata.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    School * schools = getData();
    qDebug() << schools[0].id;
    w.show();

    return a.exec();
}
