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

    qDebug() << QDir::current();

    Quad placesQuad = Quad::createUsingQlist();


    QList<Place *> schoolsList = Extractor::getSchools("schools.json");

    Node b(schoolsList[0]);

    Quad oneLocationQuad(Point(-100,-100), Point(100,100));

    oneLocationQuad.insert(&b);

    cout << "Original coordinate is" <<
            schoolsList[0]->xCoordinate<< "\n";

    cout << "node in coordinate is"
         << b.pos.x << "\n";


    qDebug() << "Searhing in big quad returns coordinate as " <<
        placesQuad.search(b.pos)->pos.x << "\n";

    qDebug() << "Searhing in small quad returns coordinate as " <<
        oneLocationQuad.search(b.pos)->pos.x << "\n";

    Place copy = *schoolsList[0];
    copy.xCoordinate = -75.3456;
    copy.yCoordinate = 13.436;
    Node anotherb(&copy);

    oneLocationQuad.insert(&anotherb);

    qDebug() <<oneLocationQuad.search(anotherb.pos)->pos.x<<endl;

    return a.exec();
}
