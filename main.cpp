#include "mainwindow.h"
#include "extractdata.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    School * schools = getSchools();
    qDebug() << schools[0].colour[0];
    HealthFacility * healthFacilities = getHealthFacilities();
    qDebug() << healthFacilities[0].id;
    w.show();

    return a.exec();
}
