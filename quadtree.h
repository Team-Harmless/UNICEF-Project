#ifndef QUADTREE_H
#define QUADTREE_H

#include <QGeoCoordinate>


#include <iostream>
#include <cmath>
#include "place.h"
#include "extractdata.h"

using namespace std;

// The objects that we want stored in the quadtree
struct Node
{
    QGeoCoordinate position;
    QList<Place*> places;
    Node(QGeoCoordinate givenPosition, QList<Place*> places);
    Node();
};

// The main quadtree class
class Quad
{
    // Hold details of the boundary of this node
    QGeoCoordinate bottomLeftPoint;
    QGeoCoordinate topRightPoint;

    // Contains details of node
    Node *nodePtr;

    // Children of this tree
    Quad *topLeftTree;
    Quad *topRightTree;
    Quad *bottomLeftTree;
    Quad *bottomRightTree;

    // height of tree
    int height;

public:
    Quad();
    Quad(QGeoCoordinate givenBottomLeft, QGeoCoordinate givenTopRight, int depth, QList<Place*> givenPlaces);
    static Quad createUsingQlist();
    static QGeoCoordinate findMidPoint(QGeoCoordinate firstPoint
        , QGeoCoordinate secondPoint);
    void insert(Node*);
    Node* search(QGeoCoordinate);
    bool inBoundary(QGeoCoordinate);
};

#endif // QUADTREE_H
