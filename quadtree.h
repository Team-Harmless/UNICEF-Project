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
    QGeoCoordinate bottomLeft;
    QGeoCoordinate topRight;

    // Contains details of node
    Node *nodePtr;

    // Children of this tree
    Quad *topLeftTree;
    Quad *topRightTree;
    Quad *botLeftTree;
    Quad *botRightTree;

public:
    Quad();
    Quad(QGeoCoordinate givenBottomLeft, QGeoCoordinate givenTopRight, int depth, QList<Place*> givenPlaces);
    static Quad createUsingQlist();
    void insert(Node*);
    Node* search(QGeoCoordinate);
    bool inBoundary(QGeoCoordinate);
};

#endif // QUADTREE_H
