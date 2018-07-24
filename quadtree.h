#ifndef QUADTREE_H
#define QUADTREE_H

#include <QGeoCoordinate>
#include <QSet>
#include <QPair>


#include <iostream>
#include <cmath>
#include "place.h"
class Extractor;

using namespace std;

// The main quadtree class
class Quad
{
    // Hold details of the boundary of this node
    QGeoCoordinate bottomLeftPoint;
    QGeoCoordinate topRightPoint;

    // Contains details of node
    QSet<Place *> *heldPlacesPtr;

    // Children of this tree
    Quad *topLeftTree;
    Quad *topRightTree;
    Quad *bottomLeftTree;
    Quad *bottomRightTree;

    // height of tree
    int height;

public:
    Quad();
    ~Quad();
    Quad(QSet<Place*> places);
    Quad(QGeoCoordinate givenBottomLeft, QGeoCoordinate givenTopRight, int depth, QSet<Place*> givenPlaces);
    QSet<Place *> search(QGeoCoordinate givenBottomLeft
                        , QGeoCoordinate givenTopRight);
    static Quad createUsingQlist();
    static QGeoCoordinate findMidPoint(QGeoCoordinate firstPoint
        , QGeoCoordinate secondPoint);
    static bool inBoundary (QGeoCoordinate givenPoint
        , QGeoCoordinate bottomLeftPoint, QGeoCoordinate topRightPoint);
    bool inBoundary(QGeoCoordinate givenPoint);
    bool isOutsideTopRight(QGeoCoordinate givenPoint);
    bool isOutsideBottomLeft(QGeoCoordinate givenPoint);
    QPair<QGeoCoordinate, QGeoCoordinate> adjustSearchBoundaries(
            QGeoCoordinate givenBottomLeft, QGeoCoordinate givenTopRight);

};

#endif // QUADTREE_H
