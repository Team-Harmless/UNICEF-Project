#include "quadtree.h"
#include <QDebug>

/*Quad Quad::createUsingQlist()
{
    QSet<Place *> schoolsList = Extractor::getSchools("schools.json");

    // actually bottomleft
    QGeoCoordinate topRightPoint = QGeoCoordinate(schoolsList[0]->xCoordinate, schoolsList[0]->yCoordinate);

    // actually topRight
    QGeoCoordinate bottomRightPoint = QGeoCoordinate(schoolsList[0]->xCoordinate, schoolsList[0]->yCoordinate);

    foreach (Place* schoolPointer, schoolsList)
    {
        if (schoolPointer->xCoordinate + schoolPointer->yCoordinate < topRightPoint.x + topRightPoint.y)
            topRightPoint = QGeoCoordinate(schoolPointer->xCoordinate, schoolPointer->yCoordinate);

        if (schoolPointer->xCoordinate + schoolPointer->yCoordinate > bottomRightPoint.x + bottomRightPoint.y)
            bottomRightPoint = QGeoCoordinate(schoolPointer->xCoordinate, schoolPointer->yCoordinate);
    } // foreach

    qDebug() << topRightPoint.x << " "<< topRightPoint.y<<endl;
    qDebug() << bottomRightPoint.x << " "<< bottomRightPoint.y<<endl;


    Quad quadPlaces(Point(-1000, 1000), Point(1000, -1000));

    foreach (Place* schoolPointer, schoolsList)
    {
        Node node(schoolPointer);
        quadPlaces.insert(&node);
    } // foreach

    Node b(schoolsList[0]);

    qDebug() << "Searhing intenally in big quad returns coordinate as " <<
        quadPlaces.search(b.pos)->pos.x << "\n";

}*/

Quad::Quad()
{
    topRightPoint= QGeoCoordinate(0, 0);
    bottomLeftPoint = QGeoCoordinate(0, 0);
    heldPlacesPtr = nullptr;
    topLeftTree  = nullptr;
    topRightTree = nullptr;
    bottomLeftTree  = nullptr;
    bottomRightTree = nullptr;
}

/* TODO - not working
Quad:: ~Quad()
{
  qDebug() << "deleting at height" << height;
  qDebug() << bottomLeftPoint << topRightPoint;
  if (topLeftTree)
      delete topLeftTree;
  if (topRightTree)
      delete topRightTree;
  if (bottomLeftTree)
      delete topLeftTree;
  if (bottomRightTree)
      delete bottomRightTree;

  qDebug() << "deleted trees at height" << height;
  if (heldPlacesPtr)
  {
      qDebug() << "no null pointer for set";
      delete heldPlacesPtr;
  }
  qDebug() << "deleted pointer at height" << height;
} */

Quad::Quad(QGeoCoordinate givenBottomLeft, QGeoCoordinate givenTopRight
           , int givenHeight, QSet<Place *> givenPlaces)
{
    heldPlacesPtr = nullptr;
    bottomLeftPoint = givenBottomLeft;
    topRightPoint = givenTopRight;
    height = givenHeight;

    // Base case - do not create new trees and populate list.
    if (height == 0)
    {
        topLeftTree = nullptr;
        topRightTree = nullptr;
        bottomLeftTree = nullptr;
        bottomRightTree = nullptr;

        heldPlacesPtr = new QSet<Place *>();

        foreach (Place * placePtr, givenPlaces)
            if (inBoundary(placePtr->coord))
       return;
    } // if

    // Step case create new trees and don't populate lists.

    // Create additional points in order to make subtrees.
    QGeoCoordinate topLeftPoint
            (topRightPoint.latitude(), bottomLeftPoint.longitude());
    QGeoCoordinate bottomRightPoint
            (bottomLeftPoint.latitude(), topRightPoint.longitude());
    QGeoCoordinate topMidPoint =
            findMidPoint(topLeftPoint, topRightPoint);
    QGeoCoordinate bottomMidPoint =
            findMidPoint(bottomLeftPoint, bottomRightPoint);
    QGeoCoordinate leftMidPoint =
            findMidPoint(bottomLeftPoint, topLeftPoint);
    QGeoCoordinate rightMidPoint =
            findMidPoint(bottomRightPoint, topRightPoint);
    QGeoCoordinate centrePoint =
            findMidPoint(leftMidPoint, rightMidPoint);

    // Create subtrees.
    topLeftTree = new Quad
            (leftMidPoint, topMidPoint, givenHeight - 1, givenPlaces);
    topRightTree = new Quad
            (centrePoint, topRightPoint, givenHeight - 1, givenPlaces);
    bottomLeftTree = new Quad
            (bottomLeftPoint, centrePoint, givenHeight - 1, givenPlaces);
    bottomRightTree = new Quad
            (bottomMidPoint, rightMidPoint, givenHeight - 1, givenPlaces);
}

QSet<Place *> Quad::search(QGeoCoordinate givenBottomLeft
                           , QGeoCoordinate givenTopRight)
{
    QSet<Place*> foundPlaces;

    if (height == 0)
    {
        QSet<Place*> foundPlaces;
        foreach (Place *placePtr, *heldPlacesPtr)
            if (inBoundary(placePtr->coord))
                foundPlaces.insert(placePtr);
        return foundPlaces;
    } // if

    // Step case.

    // For each subtree adjust search boundaries to match its size
    // and invoke search on it
    QPair<QGeoCoordinate, QGeoCoordinate> topLeftCoordinates
            = adjustSearchBoundaries(givenBottomLeft, givenTopRight);
    foundPlaces = topLeftTree->
            search(topLeftCoordinates.first, topLeftCoordinates.second);

    QPair<QGeoCoordinate, QGeoCoordinate> topRightCoordinates
            = adjustSearchBoundaries(givenBottomLeft, givenTopRight);
    foundPlaces.unite(topRightTree->search(topRightCoordinates.first
                                           , topRightCoordinates.second));

    QPair<QGeoCoordinate, QGeoCoordinate> bottomLeftCoordinates
            = adjustSearchBoundaries(givenBottomLeft, givenTopRight);
    foundPlaces.unite(bottomLeftTree->search(bottomLeftCoordinates.first
                                             , bottomLeftCoordinates.second));

    QPair<QGeoCoordinate, QGeoCoordinate> bottomRightCoordinates
            = adjustSearchBoundaries(givenBottomLeft, givenTopRight);
    foundPlaces.unite(bottomRightTree->search(bottomRightCoordinates.first
                                              , bottomRightCoordinates.second));

    return foundPlaces;
} // search

QPair<QGeoCoordinate, QGeoCoordinate> Quad::adjustSearchBoundaries(
        QGeoCoordinate givenBottomLeft, QGeoCoordinate givenTopRight)
{
    if (isOutsideBottomLeft(givenBottomLeft))
        givenBottomLeft = bottomLeftPoint;
    if(isOutsideTopRight(givenTopRight))
        givenTopRight = topRightPoint;

    return QPair<QGeoCoordinate, QGeoCoordinate> (givenBottomLeft
                                                  , givenTopRight);
}


// Check if current quadtree contains the point
bool Quad::inBoundary(QGeoCoordinate givenPoint)
{
    return inBoundary(givenPoint, bottomLeftPoint, topRightPoint);
} // non-static inBoundary

bool Quad::isOutsideTopRight(QGeoCoordinate givenPoint)
{
    return givenPoint.longitude() > topRightPoint.longitude()
            || givenPoint.latitude() > topRightPoint.latitude();
}

bool Quad::isOutsideBottomLeft(QGeoCoordinate givenPoint)
{
    return givenPoint.longitude() < bottomLeftPoint.longitude()
            || givenPoint.latitude() < bottomLeftPoint.latitude();
}
 
bool Quad::inBoundary(QGeoCoordinate givenPoint, QGeoCoordinate bottomLeftPoint
                      , QGeoCoordinate topRightPoint)
{
    return (givenPoint.longitude() >= bottomLeftPoint.longitude()
        && givenPoint.latitude() >= bottomLeftPoint.latitude()
        && givenPoint.longitude() <= topRightPoint.longitude()
        && givenPoint.latitude() <= topRightPoint.latitude());
} // static inBoundary

QGeoCoordinate Quad::findMidPoint(QGeoCoordinate firstPoint
    , QGeoCoordinate secondPoint)
{
  // MidPoint( (x1 + x2) / 2, (y1 + y2) / 2 )
  return QGeoCoordinate((firstPoint.latitude() + secondPoint.latitude()) / 2
                        , (firstPoint.longitude() + secondPoint.longitude()) / 2);
} // findMidPoint


// Driver program
/*int main()
{
    Quad center(Point(0, 0), Point(8, 8));
    Node a(Point(1, 1), 1);
    Node b(Point(2, 5), 2);
    Node c(Point(7, 6), 3);
    center.insert(&a);
    center.insert(&b);
    center.insert(&c);
    cout << "Node a: " <<
        center.search(Point(1, 1))->data << "\n";
    cout << "Node b: " <<
        center.search(Point(2, 5))->data << "\n";
    cout << "Node c: " <<
        center.search(Point(7, 6))->data << "\n";
    cout << "Non-existing node: "
        << center.search(Point(5, 5));
    return 0;
}*/


/*
 * // Insert a node into the quadtree
void Quad::insert(Node *node)
{
    if (node == nullptr)
        return;

    // Current quad cannot contain it
    if (!inBoundary(node->pos))
        return;

    // We are at a quad of unit area
    // We cannot subdivide this quad further
    if (abs(topLeft.x - botRight.x) <= 1 &&
        abs(topLeft.y - botRight.y) <= 1)
    {
        if (n == nullptr)
            n = node;
        return;
    }

    if ((topLeft.x + botRight.x) / 2 >= node->pos.x)
    {
        // Indicates topLeftTree
        if ((topLeft.y + botRight.y) / 2 >= node->pos.y)
        {
            if (topLeftTree == nullptr)
                topLeftTree = new Quad(
                    Point(topLeft.x, topLeft.y),
                    Point((topLeft.x + botRight.x) / 2,
                        (topLeft.y + botRight.y) / 2));
            topLeftTree->insert(node);
        }

        // Indicates botLeftTree
        else
        {
            if (botLeftTree == nullptr)
                botLeftTree = new Quad(
                    Point(topLeft.x,
                        (topLeft.y + botRight.y) / 2),
                    Point((topLeft.x + botRight.x) / 2,
                        botRight.y));
            botLeftTree->insert(node);
        }
    }
    else
    {
        // Indicates topRightTree
        if ((topLeft.y + botRight.y) / 2 >= node->pos.y)
        {
            if (topRightTree == nullptr)
                topRightTree = new Quad(
                    Point((topLeft.x + botRight.x) / 2,
                        topLeft.y),
                    Point(botRight.x,
                        (topLeft.y + botRight.y) / 2));
            topRightTree->insert(node);
        }

        // Indicates botRightTree
        else
        {
            if (botRightTree == nullptr)
                botRightTree = new Quad(
                    Point((topLeft.x + botRight.x) / 2,
                        (topLeft.y + botRight.y) / 2),
                    Point(botRight.x, botRight.y));
            botRightTree->insert(node);
        }
    }
}

// Find a node in a quadtree
Node* Quad::search(Point p)
{
    // Current quad cannot contain it
    if (!inBoundary(p))
        return nullptr;

    // We are at a quad of unit length
    // We cannot subdivide this quad further
    if (n != nullptr)
        return n;

    if ((topLeft.x + botRight.x) / 2 >= p.x)
    {
        // Indicates topLeftTree
        if ((topLeft.y + botRight.y) / 2 >= p.y)
        {
            if (topLeftTree == nullptr)
                return nullptr;
            return topLeftTree->search(p);
        }

        // Indicates botLeftTree
        else
        {
            if (botLeftTree == nullptr)
                return nullptr;
            return botLeftTree->search(p);
        }
    }
    else
    {
        // Indicates topRightTree
        if ((topLeft.y + botRight.y) / 2 >= p.y)
        {
            if (topRightTree == nullptr)
                return nullptr;
            return topRightTree->search(p);
        }

        // Indicates botRightTree
        else
        {
            if (botRightTree == nullptr)
                return nullptr;
            return botRightTree->search(p);
        }
    }
};

// Check if current quadtree contains the point
bool Quad::inBoundary(Point p)
{
    return (p.x >= topLeft.x &&
        p.x <= botRight.x &&
        p.y >= topLeft.y &&
        p.y <= botRight.y);
}

// Driver program
int main()
{
    Quad center(Point(0, 0), Point(8, 8));
    Node a(Point(1, 1), 1);
    Node b(Point(2, 5), 2);
    Node c(Point(7, 6), 3);
    center.insert(&a);
    center.insert(&b);
    center.insert(&c);
    cout << "Node a: " <<
        center.search(Point(1, 1))->data << "\n";
    cout << "Node b: " <<
        center.search(Point(2, 5))->data << "\n";
    cout << "Node c: " <<
        center.search(Point(7, 6))->data << "\n";
    cout << "Non-existing node: "
        << center.search(Point(5, 5));
    return 0;
}*/
