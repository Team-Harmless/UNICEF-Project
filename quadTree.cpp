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
    topRight= QGeoCoordinate(0, 0);
    bottomLeft = QGeoCoordinate(0, 0);
    nodePtr = nullptr;
    topLeftTree  = nullptr;
    topRightTree = nullptr;
    botLeftTree  = nullptr;
    botRightTree = nullptr;
}
Quad::Quad(QGeoCoordinate givenBottomLeft, QGeoCoordinate givenTopRight, int depth, QList<Place *> givenPlaces)
{
    nodePtr = nullptr;
    topLeftTree  = nullptr;
    topRightTree = nullptr;
    botLeftTree  = nullptr;
    botRightTree = nullptr;
    bottomLeft = givenBottomLeft;
    topRight = givenTopRight;
    depth  = 1;
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
    if (abs(topRight.latitude() - bottomLeft.latitude()) <= 1 &&
        abs(topRight.longitude() - bottomLeft.longitude()) <= 1)
    {
        if (nodePtr== nullptr)
            nodePtr= node;
        return;
    }
 
    if ((topRight.latitude() + bottomLeft.latitude()) / 2 >= node->position.latitude())
    {
        // Indicates topLeftTree
        if ((topRight.longitude() + bottomLeft.longitude()) / 2 >= node->position.longitude())
        {
            if (topLeftTree == nullptr)
                topLeftTree = new Quad(
                    QGeoCoordinate(topRight.latitude(), topRight.longitude()),
                    QGeoCoordinate((topRight.latitude() + bottomLeft.latitude()) / 2,
                        (topRight.longitude() + bottomLeft.longitude()) / 2));
            topLeftTree->insert(node);
        }
 
        // Indicates botLeftTree
        else
        {
            if (botLeftTree == nullptr)
                botLeftTree = new Quad(
                    QGeoCoordinate(topRight.latitude(),
                        (topRight.longitude() + bottomLeft.longitude()) / 2),
                    QGeoCoordinate((topRight.latitude() + bottomLeft.latitude()) / 2,
                        bottomLeft.longitude()));
            botLeftTree->insert(node);
        }
    }
    else
    {
        // Indicates topRightTree
        if ((topRight.longitude() + bottomLeft.longitude()) / 2 >= node->position.longitude())
        {
            if (topRightTree == nullptr)
                topRightTree = new Quad(
                    QGeoCoordinate((topRight.latitude() + bottomLeft.latitude()) / 2,
                        topRight.longitude()),
                    QGeoCoordinate(bottomLeft.latitude(),
                        (topRight.longitude() + bottomLeft.longitude()) / 2));
            topRightTree->insert(node);
        }
 
        // Indicates botRightTree
        else
        {
            if (botRightTree == nullptr)
                botRightTree = new Quad(
                    QGeoCoordinate((topRight.latitude() + bottomLeft.latitude()) / 2,
                        (topRight.longitude() + bottomLeft.longitude()) / 2),
                    QGeoCoordinate(bottomLeft.latitude(), bottomLeft.longitude()));
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
 
    if ((topRight.latitude() + bottomLeft.latitude()) / 2 >= p.latitude())
    {
        // Indicates topLeftTree
        if ((topRight.longitude() + bottomLeft.longitude()) / 2 >= p.longitude())
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
        if ((topRight.longitude() + bottomLeft.longitude()) / 2 >= p.longitude())
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
bool Quad::inBoundary(QGeoCoordinate p)
{
    return (p.latitude() >= topRight.latitude() &&
        p.latitude() <= bottomLeft.latitude() &&
        p.longitude() >= topRight.longitude() &&
        p.longitude() <= bottomLeft.longitude());
}
 
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
