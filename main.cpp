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
    double minLongtitude = firstSchool->coord.longitude();
    double maxLongtitude = firstSchool->coord.longitude();
    double minLatitude = firstSchool->coord.latitude();
    double maxLatitude = firstSchool->coord.latitude();

    qDebug() << "len is " << schoolsList.size();
    qDebug() << "first x is" << firstSchool->coord.latitude() <<endl;
    foreach (Place* schoolPointer, schoolsList)
    {
        qDebug() << "x is" << schoolPointer->coord.latitude()<< endl;
        if (minLongtitude < schoolPointer->coord.longitude())
            minLongtitude = schoolPointer->coord.longitude();
        if (maxLongtitude > schoolPointer->coord.longitude())
            maxLongtitude = schoolPointer->coord.longitude();
        if (minLatitude < schoolPointer->coord.latitude())
            minLatitude = schoolPointer->coord.latitude();
        if (maxLatitude > schoolPointer->coord.latitude())
            maxLatitude = schoolPointer->coord.latitude();
    } // foreach

    qDebug() << "Min and max x are " << minLongtitude << " " <<maxLongtitude<<endl;
    qDebug() << "Min and max y are " << minLatitude << " " <<maxLatitude<<endl;

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
