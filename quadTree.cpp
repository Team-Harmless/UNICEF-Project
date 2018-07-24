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

Quad:: ~Quad()
{
  if (topLeftTree)
  {
      delete topLeftTree;
      topLeftTree = nullptr;
  }
  if (topRightTree)
  {
      delete topRightTree;
      topLeftTree = nullptr;
  }
  if (bottomLeftTree)
  {
      delete topLeftTree;
      topLeftTree = nullptr;
  }
  if (bottomRightTree)
  {
      delete bottomRightTree;
      topLeftTree = nullptr;
  }

  if (heldPlacesPtr)
  {
      delete heldPlacesPtr;
  }

  heldPlacesPtr = nullptr;
}

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
                heldPlacesPtr->insert(placePtr);
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



/*
QList<Place *> schoolsList = Extractor::getSchools("schools.json");

Place * firstSchool  = schoolsList.takeAt(0);
double minLongitude = firstSchool->coord.longitude();
double maxLongitude = firstSchool->coord.longitude();
double minLatitude = firstSchool->coord.latitude();
double maxLatitude = firstSchool->coord.latitude();

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

QGeoCoordinate bottomLeft(minLatitude, minLongitude);
QGeoCoordinate topRight(maxLatitude, maxLongitude);

Quad *schoolsQuad = new Quad(bottomLeft, topRight, 3, schoolsList.toSet());


QSet<Place*> results = schoolsQuad->search(QGeoCoordinate(-5, -79), QGeoCoordinate(13,0));

qDebug() << results.size();

int count = 0;
foreach (Place * placePtr, results)
    if (count >= 100)
        break;
    else
    {
        qDebug() << placePtr->name;
        count++;
    }
*/
