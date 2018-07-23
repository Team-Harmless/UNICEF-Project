#include "quadtree.h"
#include <QDebug>

/*Quad Quad::createUsingQlist()
{
    QList<Place *> schoolsList = Extractor::getSchools("schools.json");

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

    return quadPlaces;
}*/

Node::Node(QGeoCoordinate givenPosistion, QList<Place*> givenPlaces)
{
    position = givenPosistion;
    places = givenPlaces;
}


Node:: Node()
{
    position = QGeoCoordinate(0,0);
    places = QList<Place*>();
}


Quad::Quad()
{
    topRightPoint= QGeoCoordinate(0, 0);
    bottomLeftPoint = QGeoCoordinate(0, 0);
    nodePtr = nullptr;
    topLeftTree  = nullptr;
    topRightTree = nullptr;
    bottomLeftTree  = nullptr;
    bottomRightTree = nullptr;
}

Quad::Quad(QGeoCoordinate givenBottomLeft, QGeoCoordinate givenTopRight
           , int givenHeight, QList<Place *> givenPlaces)
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

        heldPlacesPtr = new QList<Place *>();

        foreach (Place * placePtr, givenPlaces)
            if (inBoundary(placePtr->coordinate))
                heldPlacesPtr->append(placePtr);
       return;
    } // if

    // Step case create new trees and don't populate lists.

    // Create additional points in order to make subtrees.
    QGeoCoordinate topLeftPoint
            (bottomLeftPoint.longitude(), topRightPoint.latitude());
    QGeoCoordinate bottomRightPoint
            (topRightPoint.longitude(), bottomLeftPoint.latitude());
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

// Insert a node into the quadtree
void Quad::insert(Node *node)
{
    if (node == nullptr)
        return;
 
    // Current quad cannot contain it
    if (!inBoundary(node->position))
        return;
 
    // We are at a quad of unit area
    // We cannot subdivide this quad further
    if (abs(topRightPoint.latitude() - bottomLeftPoint.latitude()) <= 1 &&
        abs(topRightPoint.longitude() - bottomLeftPoint.longitude()) <= 1)
    {
        if (nodePtr== nullptr)
            nodePtr= node;
        return;
    }
 
    if ((topRightPoint.latitude() + bottomLeftPoint.latitude()) / 2 >= node->position.latitude())
    {
        // Indicates topLeftTree
        if ((topRightPoint.longitude() + bottomLeftPoint.longitude()) / 2 >= node->position.longitude())
        {
            if (topLeftTree == nullptr)
                topLeftTree = new Quad(
                    QGeoCoordinate(topRightPoint.latitude(), topRightPoint.longitude()),
                    QGeoCoordinate((topRightPoint.latitude() + bottomLeftPoint.latitude()) / 2,
                        (topRightPoint.longitude() + bottomLeftPoint.longitude()) / 2));
            topLeftTree->insert(node);
        }
 
        // Indicates botLeftTree
        else
        {
            if (botLeftTree == nullptr)
                botLeftTree = new Quad(
                    QGeoCoordinate(topRightPoint.latitude(),
                        (topRightPoint.longitude() + bottomLeftPoint.longitude()) / 2),
                    QGeoCoordinate((topRightPoint.latitude() + bottomLeftPoint.latitude()) / 2,
                        bottomLeftPoint.longitude()));
            botLeftTree->insert(node);
        }
    }
    else
    {
        // Indicates topRightTree
        if ((topRightPoint.longitude() + bottomLeftPoint.longitude()) / 2 >= node->position.longitude())
        {
            if (topRightPointTree == nullptr)
                topRightPointTree = new Quad(
                    QGeoCoordinate((topRightPoint.latitude() + bottomLeftPoint.latitude()) / 2,
                        topRightPoint.longitude()),
                    QGeoCoordinate(bottomLeftPoint.latitude(),
                        (topRight.longitude() + bottomLeftPoint.longitude()) / 2));
            topRightTree->insert(node);
        }
 
        // Indicates botRightTree
        else
        {
            if (botRightTree == nullptr)
                botRightTree = new Quad(
                    QGeoCoordinate((topRight.latitude() + bottomLeftPoint.latitude()) / 2,
                        (topRight.longitude() + bottomLeftPoint.longitude()) / 2),
                    QGeoCoordinate(bottomLeftPoint.latitude(), bottomLeftPoint.longitude()));
            botRightTree->insert(node);
        }
    }
}
 
// Find a node in a quadtree
Node* Quad::search(QGeoCoordinate p)
{
    // Current quad cannot contain it
    if (!inBoundary(p))
        return nullptr;
 
    // We are at a quad of unit length
    // We cannot subdivide this quad further
    if (nodePtr!= nullptr)
        return n;
 
    if ((topRight.latitude() + bottomLeftPoint.latitude()) / 2 >= p.latitude())
    {
        // Indicates topLeftTree
        if ((topRight.longitude() + bottomLeftPoint.longitude()) / 2 >= p.longitude())
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
        if ((topRight.longitude() + bottomLeftPoint.longitude()) / 2 >= p.longitude())
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
bool Quad::inBoundary(QGeoCoordinate givenPoint)
{
    // Is the point "after" the bottom left corner and "before"
    // the top right corner (or on the edge of the map)? 
    return (givenPoint.longitude() >= bottomLeftPoint.longitude()
        && givenPoint.latitude() >= bottomLeftPoint.latitude()
        && givenPoint.longitude() <= topRightPoint.longitude()
        && givenPoint.latitude() <= topRightPoint.latitude());
}
 
QGeoCoordinate Quad::findMidPoint(QGeoCoordinate firstPoint
    , QGeoCoordinate secondPoint)
{
  // MidPoint( (x1 + x2) / 2, (y1 + y2) / 2 )
  return QGeoCoordinate((firstPoint.longitude() + secondPoint.longitude()) / 2
      , (firstPoint.latitude() + secondPoint.latitude()) / 2);
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
