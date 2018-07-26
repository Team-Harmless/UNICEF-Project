#include "quadtree.h"
#include <QDebug>

#define HEIGHT 3

Quad::Quad()
{
    topRightPoint= QGeoCoordinate(0, 0);
    bottomLeftPoint = QGeoCoordinate(0, 0);
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
}

void _recAdd(Quad *quad, Place* place, int height) {
    if (height == 0) {
        quad->heldPlacesPtr << place;
        return;
    }
    if (quad->topLeftTree->inBoundary(place->coord)) {
        _recAdd(quad->topLeftTree, place, height-1);
        return;
    }
    if (quad->topRightTree->inBoundary(place->coord)) {
        _recAdd(quad->topRightTree, place, height-1);
        return;
    }
    if (quad->bottomLeftTree->inBoundary(place->coord)) {
        _recAdd(quad->bottomLeftTree, place, height-1);
        return;
    }
    if (quad->bottomRightTree->inBoundary(place->coord)) {
        _recAdd(quad->bottomRightTree, place, height-1);
        return;
    }
}

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

    height = HEIGHT;

    // Base case - do not create new trees and populate list.
    if (height == 0)
    {
        topLeftTree = nullptr;
        topRightTree = nullptr;
        bottomLeftTree = nullptr;
        bottomRightTree = nullptr;


//        foreach (Place * placePtr, places)
//            if (inBoundary(placePtr->coord))
//                heldPlacesPtr->insert(placePtr);
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
    foreach(Place* place, places)
        _recAdd(this, place, HEIGHT);
}

Quad::Quad(QGeoCoordinate givenBottomLeft, QGeoCoordinate givenTopRight
           , int givenHeight, QSet<Place *> givenPlaces)
{
    bottomLeftPoint = givenBottomLeft;
    topRightPoint = givenTopRight;
    height = givenHeight;

    // Base case - do not create new trees and populate list.
    if (givenHeight == 0)
    {
        topLeftTree = nullptr;
        topRightTree = nullptr;
        bottomLeftTree = nullptr;
        bottomRightTree = nullptr;


//        foreach (Place * placePtr, givenPlaces)
//            if (inBoundary(placePtr->coord))
//                heldPlacesPtr << placePtr;
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

QSet<Place *> Quad::search(QGeoCoordinate givenBottomLeft, QGeoCoordinate givenTopRight)
{
    return _search(givenBottomLeft, givenTopRight, HEIGHT);
}

QSet<Place *> Quad::_search(QGeoCoordinate givenBottomLeft
                           , QGeoCoordinate givenTopRight
                           , int givenHeight)
{
    QSet<Place*> foundPlaces;

    if (givenHeight == 0)
    {
        QSet<Place*> foundPlaces;
        foreach (Place *placePtr, heldPlacesPtr)
            if (inBoundary(placePtr->coord, givenBottomLeft, givenTopRight))
                foundPlaces.insert(placePtr);
        if (foundPlaces.count() > 0) qDebug() << "Found Places:" << foundPlaces.count() << " Out of " << heldPlacesPtr.count();
        //qDebug() << "Diagonal: " << givenBottomLeft.distanceTo(givenTopRight) / 1000;
        return foundPlaces;
    } // if

    // Step case.

    // For each subtree adjust search boundaries to match its size
    // and invoke search on it


    if (topLeftTree->inBoundary(QGeoCoordinate(givenBottomLeft.latitude(), givenTopRight.longitude()))) {
        QGeoCoordinate blp = QGeoCoordinate(givenBottomLeft.latitude(), topLeftTree->bottomLeftPoint.longitude());
        QGeoCoordinate trp = QGeoCoordinate(topLeftTree->topRightPoint.latitude(), givenTopRight.longitude());
        QSet<Place*> tmp = topLeftTree->_search(blp, trp, givenHeight - 1);
        foundPlaces = foundPlaces.unite(tmp);
        //qDebug() << "Places found in TL at hight " << givenHeight << " is " << foundPlaces.count();
    }

    if (bottomLeftTree->inBoundary(givenBottomLeft)) {
        QGeoCoordinate blp = givenBottomLeft;
        QGeoCoordinate trp = bottomLeftTree->topRightPoint;
        QSet<Place*> tmp = bottomLeftTree->_search(blp, trp, givenHeight - 1);
        foundPlaces = foundPlaces.unite(tmp);
        //qDebug() << "Places found in BL ah hight " << givenHeight << " is " << foundPlaces.count();
    }

    if (topRightTree->inBoundary(givenTopRight)) {
        QGeoCoordinate blp = topRightTree->bottomLeftPoint;
        QGeoCoordinate trp = givenTopRight;
        //foundPlaces.unite(topRightTree->_search(blp, trp, givenHeight - 1));
        QSet<Place*> tmp = topRightTree->_search(blp, trp, givenHeight - 1);
        foundPlaces = foundPlaces.unite(tmp);
        //qDebug() << "Places found in TR ah hight " << givenHeight << " is " << foundPlaces.count();
    }

    if (bottomRightTree->inBoundary(QGeoCoordinate(givenTopRight.latitude(), givenBottomLeft.longitude()))) {
        QGeoCoordinate blp = QGeoCoordinate(bottomRightTree->bottomLeftPoint.latitude(), givenBottomLeft.longitude());
        QGeoCoordinate trp = QGeoCoordinate(givenTopRight.latitude(), bottomRightTree->topRightPoint.longitude());
        //foundPlaces.unite(bottomRightTree->_search(blp, trp, givenHeight - 1));
        QSet<Place*> tmp = bottomRightTree->_search(blp, trp, givenHeight - 1);
        foundPlaces = foundPlaces.unite(tmp);
        //qDebug() << "Places found in BR ah hight " << givenHeight << " is " << foundPlaces.count();
    }



//    QPair<QGeoCoordinate, QGeoCoordinate> topLeftCoordinates
//            = adjustSearchBoundaries(givenBottomLeft, givenTopRight);
//    foundPlaces = topLeftTree->
//            _search(topLeftCoordinates.first, topLeftCoordinates.second
//                    ,givenHeight-1);

//    QPair<QGeoCoordinate, QGeoCoordinate> topRightCoordinates
//            = adjustSearchBoundaries(givenBottomLeft, givenTopRight);
//    foundPlaces.unite(topRightTree->_search(topRightCoordinates.first
//                                           , topRightCoordinates.second
//                                            ,givenHeight-1));

//    QPair<QGeoCoordinate, QGeoCoordinate> bottomLeftCoordinates
//            = adjustSearchBoundaries(givenBottomLeft, givenTopRight);
//    foundPlaces.unite(bottomLeftTree->_search(bottomLeftCoordinates.first
//                                             , bottomLeftCoordinates.second
//                                              ,givenHeight-1));

//    QPair<QGeoCoordinate, QGeoCoordinate> bottomRightCoordinates
//            = adjustSearchBoundaries(givenBottomLeft, givenTopRight);
//    foundPlaces.unite(bottomRightTree->_search(bottomRightCoordinates.first
//                                              , bottomRightCoordinates.second
//                                               ,givenHeight-1));

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
 
bool Quad::inBoundary(QGeoCoordinate givenPoint, QGeoCoordinate BottomLeftPoint
                      , QGeoCoordinate TopRightPoint)
{
    return (givenPoint.longitude() >= BottomLeftPoint.longitude()
        && givenPoint.latitude() >= BottomLeftPoint.latitude()
        && givenPoint.longitude() <= TopRightPoint.longitude()
        && givenPoint.latitude() <= TopRightPoint.latitude());
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
