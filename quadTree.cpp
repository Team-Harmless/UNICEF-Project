#include "quadtree.h"
#include <QDebug>

#define HEIGHT 3

Quad::Quad()
{
    topRightPoint= nullptr;
    bottomLeftPoint = nullptr;
    heldPlacesPtr = nullptr;
    topLeftTree  = nullptr;
    topRightTree = nullptr;
    bottomLeftTree  = nullptr;
    bottomRightTree = nullptr;
    isEmpty = new bool(true);
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
      heldPlacesPtr = nullptr;
  }
}


/*
Quad::Quad(QSet<Place*> places)
{
    double minLongitude = places.values().at(0)->coord.longitude();
    double maxLongitude = places.values().at(0)->coord.longitude();
    double minLatitude = places.values().at(0)->coord.latitude();
    double maxLatitude = places.values().at(0)->coord.latitude();

    foreach (Place* place, places)
    {
        if (minLongitude > place->coord.longitude())
            minLongitude = place->coord.longitude();
        else if (maxLongitude < place->coord.longitude())
            maxLongitude = place->coord.longitude();
        if (minLatitude > place->coord.latitude())
            minLatitude = place->coord.latitude();
        else if (maxLatitude < place->coord.latitude())
            maxLatitude = place->coord.latitude();
    } // foreach

    bottomLeftPoint = QGeoCoordinate(minLatitude, minLongitude);
    topRightPoint = QGeoCoordinate(maxLatitude, maxLongitude);

    heldPlacesPtr = nullptr;
    height = HEIGHT;

    // Base case - do not create new trees and populate list.
    if (height == 0)
    {
        topLeftTree = nullptr;
        topRightTree = nullptr;
        bottomLeftTree = nullptr;
        bottomRightTree = nullptr;

        heldPlacesPtr = new QSet<Place *>();

        foreach (Place * placePtr, places)
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
            (leftMidPoint, topMidPoint, height - 1, places);
    topRightTree = new Quad
            (centrePoint, topRightPoint, height - 1, places);
    bottomLeftTree = new Quad
            (bottomLeftPoint, centrePoint, height - 1, places);
    bottomRightTree = new Quad
            (bottomMidPoint, rightMidPoint, height - 1, places);
}
*/

Quad::Quad(QGeoCoordinate *givenBottomLeft, QGeoCoordinate *givenTopRight, int givenHeight, QSet<Place *> givenPlaces)
{
    isEmpty = new bool(true);
    heldPlacesPtr = nullptr;
    bottomLeftPoint = givenBottomLeft;
    topRightPoint = givenTopRight;
    height = new int(givenHeight);

    // Base case - do not create new trees and populate list.
    if (givenHeight == 0)
    {
        topLeftTree = nullptr;
        topRightTree = nullptr;
        bottomLeftTree = nullptr;
        bottomRightTree = nullptr;

        heldPlacesPtr = new QSet<Place *>();

        foreach (Place * placePtr, givenPlaces)
            if (inBoundary(placePtr->coord))
                heldPlacesPtr->insert(placePtr);

        if (heldPlacesPtr->isEmpty())
        {
            heldPlacesPtr = nullptr;
        } // if
        else
        {
            isEmpty = new bool(false);
        }

       return;
    } // if

    // Step case create new trees and don't populate lists.

    // Create additional points in order to make subtrees.
    QGeoCoordinate *topLeftPoint = new QGeoCoordinate(topRightPoint->latitude(), bottomLeftPoint->longitude());
    QGeoCoordinate *bottomRightPoint = new QGeoCoordinate(bottomLeftPoint->latitude(), topRightPoint->longitude());
    QGeoCoordinate *topMidPoint = new QGeoCoordinate(findMidPoint(*topLeftPoint, *topRightPoint));
    QGeoCoordinate *bottomMidPoint = new QGeoCoordinate (findMidPoint(*bottomLeftPoint, *bottomRightPoint));
    QGeoCoordinate *leftMidPoint = new QGeoCoordinate(findMidPoint(*bottomLeftPoint, *topLeftPoint));
    QGeoCoordinate *rightMidPoint = new QGeoCoordinate(findMidPoint(*bottomRightPoint, *topRightPoint));
    QGeoCoordinate *centrePoint = new QGeoCoordinate(findMidPoint(*leftMidPoint, *rightMidPoint));

    // Create subtrees.
    topLeftTree = new Quad
            (leftMidPoint, topMidPoint, givenHeight - 1, givenPlaces);
    topRightTree = new Quad
            (centrePoint, topRightPoint, givenHeight - 1, givenPlaces);
    bottomLeftTree = new Quad
            (bottomLeftPoint, centrePoint, givenHeight - 1, givenPlaces);
    bottomRightTree = new Quad
            (bottomMidPoint, rightMidPoint, givenHeight - 1, givenPlaces);
    isEmpty =  new bool(!(!*(topLeftTree->isEmpty) || !*(topRightTree->isEmpty) || !*(bottomLeftTree->isEmpty)|| !*(bottomRightTree->isEmpty)));
}

QSet<Place *> Quad::search(QGeoCoordinate *givenBottomLeft, QGeoCoordinate *givenTopRight)
{
    return _search(givenBottomLeft, givenTopRight, HEIGHT);
}

QSet<Place *> Quad::_search(QGeoCoordinate *givenBottomLeft
                           , QGeoCoordinate *givenTopRight
                           , int givenHeight)
{
    QSet<Place*> foundPlaces = QSet<Place*>();

    if (givenHeight == 0)
    {
        if (!heldPlacesPtr)
        if (heldPlacesPtr == nullptr) return QSet<Place*>();
        QSet<Place*> foundPlaces;
        foreach (Place *placePtr, *heldPlacesPtr)
            if (placePtr != nullptr)
                if (inBoundary(placePtr->coord))
                    foundPlaces.insert(placePtr);
        return foundPlaces;
    } // if

    // Step case.

    // For each subtree adjust search boundaries to match its size
    // and invoke search on it
    if (!*(topLeftTree->isEmpty))
    {

    QPair<QGeoCoordinate*, QGeoCoordinate*> topLeftCoordinates
            = adjustSearchBoundaries(givenBottomLeft, givenTopRight);
    foundPlaces = topLeftTree->
            _search(topLeftCoordinates.first, topLeftCoordinates.second
                    ,givenHeight-1);
    }

    if (!*(topRightTree->isEmpty))
    {
    QPair<QGeoCoordinate*, QGeoCoordinate*> topRightCoordinates
            = adjustSearchBoundaries(givenBottomLeft, givenTopRight);
    foundPlaces.unite(topRightTree->_search(topRightCoordinates.first
                                           , topRightCoordinates.second
                                            ,givenHeight-1));
    }

    if (!*(bottomLeftTree->isEmpty))
    {
    QPair<QGeoCoordinate*, QGeoCoordinate*> bottomLeftCoordinates
            = adjustSearchBoundaries(givenBottomLeft, givenTopRight);
    foundPlaces.unite(bottomLeftTree->_search(bottomLeftCoordinates.first
                                             , bottomLeftCoordinates.second
                                              ,givenHeight-1));
    }

    if (!*(bottomRightTree->isEmpty))
    {
    QPair<QGeoCoordinate*, QGeoCoordinate*> bottomRightCoordinates
            = adjustSearchBoundaries(givenBottomLeft, givenTopRight);
    foundPlaces.unite(bottomRightTree->_search(bottomRightCoordinates.first
                                              , bottomRightCoordinates.second
                                               ,givenHeight-1));
    }

    return foundPlaces;
} // search

QPair<QGeoCoordinate*, QGeoCoordinate*> Quad::adjustSearchBoundaries(
        QGeoCoordinate *givenBottomLeft, QGeoCoordinate *givenTopRight)
{
    if (isOutsideBottomLeft(*givenBottomLeft))
        givenBottomLeft = bottomLeftPoint;
    if(isOutsideTopRight(*givenTopRight))
        givenTopRight = topRightPoint;

    return QPair<QGeoCoordinate*, QGeoCoordinate*> (givenBottomLeft
                                                  , givenTopRight);
}


// Check if current quadtree contains the point
bool Quad::inBoundary(QGeoCoordinate givenPoint)
{
    return inBoundary(givenPoint, *bottomLeftPoint, *topRightPoint);
} // non-static inBoundary

bool Quad::isOutsideTopRight(QGeoCoordinate givenPoint)
{
    return givenPoint.longitude() > topRightPoint->longitude()
            || givenPoint.latitude() > topRightPoint->latitude();
}

bool Quad::isOutsideBottomLeft(QGeoCoordinate givenPoint)
{
    return givenPoint.longitude() < bottomLeftPoint->longitude()
            || givenPoint.latitude() < bottomLeftPoint->latitude();
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


int count = 0;
foreach (Place * placePtr, results)
    if (count >= 100)
        break;
    else
    {
        count++;
    }
*/
