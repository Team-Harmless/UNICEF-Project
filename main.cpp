#include "mainwindow.h"
#include "quadTree.cpp"
#include <QApplication>
#include <QDir>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    /*
    qDebug() << QDir::current();

    Quad placesQuad = Quad::createUsingQlist();
    Place * firstSchool  = schoolsList.takeAt(0);
    double minLongitude = firstSchool->coord.longitude();
    double maxLongitude = firstSchool->coord.longitude();
    double minLatitude = firstSchool->coord.latitude();
    double maxLatitude = firstSchool->coord.latitude();

    foreach (Place* schoolPointer, schoolsList)
    {
        if (minLongitude > schoolPointer->coord.longitude())
            minLongitude = schoolPointer->coord.longitude();
        else if (maxLongitude < schoolPointer->coord.longitude())
            maxLongitude = schoolPointer->coord.longitude();
        if (minLatitude > schoolPointer->coord.latitude())
            minLatitude = schoolPointer->coord.latitude();
        else if (maxLatitude < schoolPointer->coord.latitude())
            maxLatitude = schoolPointer->coord.latitude();
    } // foreach

    QGeoCoordinate bottomLeft(minLatitude, minLongitude);
    QGeoCoordinate topRight(maxLatitude, maxLongitude);

    Quad *schoolsQuad = new Quad(bottomLeft, topRight, 3, schoolsList.toSet());


    QSet<Place*> results = schoolsQuad->search(QGeoCoordinate(-5, -79), QGeoCoordinate(13,0));
    */
    return a.exec();
}
