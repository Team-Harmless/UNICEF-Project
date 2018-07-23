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



    QList<Place *> schoolsList = Extractor::getSchools("schools.json");

    Place * firstSchool  = schoolsList.takeAt(0);
    double minLongitude = firstSchool->coord.longitude();
    double maxLongitude = firstSchool->coord.longitude();
    double minLatitude = firstSchool->coord.latitude();
    double maxLatitude = firstSchool->coord.latitude();

    qDebug() << "len is " << schoolsList.size();
    qDebug() << "first x is" << firstSchool->coord.latitude() <<endl;
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

    qDebug() << "Min and max x are " << minLongitude << " " <<maxLongitude<<endl;
    qDebug() << "Min and max y are " << minLatitude << " " <<maxLatitude;

    /*
    Quad placesQuad = Quad::();

    cout << "Original coordinate is" <<
            schoolsList[0]->xCoordinate<< "\n";

    cout << "node in coordinate is"
         << b.pos.x << "\n";


    qDebug() << "Searhing in big quad returns coordinate as " <<
        placesQuad.search(b.pos)->pos.x << "\n";

    qDebug() << "Searhing in small quad returns coordinate as " <<
        oneLocationQuad.search(b.pos)->pos.x << "\n";
        */

    return a.exec();
}
