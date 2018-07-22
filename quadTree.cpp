#include <iostream>
#include <cmath>
#include "place.h"
using namespace std;
 
// Used to hold details of a point
struct Point
{
    double x;
    double y;
    Point(double _x, double _y)
    {
        x = _x;
        y = _y;
    }
    Point()
    {
        x = 0;
        y = 0;
    }
};
 
// The objects that we want stored in the quadtree
struct Node
{
    Point pos;
    Place * place;
    Node(Place * givenPlace)
    {
        double x = givenPlace->xCoordinate;
        double y = givenPlace->yCoordinate;
        pos = Point(x,y);
        place = givenPlace;
    }
    Node()
    {
        place = nullptr;
    }
};
 
// The main quadtree class
class Quad
{
    // Hold details of the boundary of this node
    Point topLeft;
    Point botRight;
 
    // Contains details of node
    Node *n;
 
    // Children of this tree
    Quad *topLeftTree;
    Quad *topRightTree;
    Quad *botLeftTree;
    Quad *botRightTree;
 
public:
    Quad()
    {
        topLeft = Point(0, 0);
        botRight = Point(0, 0);
        n = nullptr;
        topLeftTree  = nullptr;
        topRightTree = nullptr;
        botLeftTree  = nullptr;
        botRightTree = nullptr;
    }
    Quad(Point topL, Point botR)
    {
        n = nullptr;
        topLeftTree  = nullptr;
        topRightTree = nullptr;
        botLeftTree  = nullptr;
        botRightTree = nullptr;
        topLeft = topL;
        botRight = botR;
    }
    void insert(Node*);
    Node* search(Point);
    bool inBoundary(Point);
};
 
// Insert a node into the quadtree
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
