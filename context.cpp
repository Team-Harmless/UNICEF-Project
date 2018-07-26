#include "context.h"
#include <QDebug>

QList<Polar> Context::getAllTheJucyData()
{
    return polarCoordinates;
}

Context::Context()
{
    origin = nullptr;
    polarCoordinates = QList<Polar>();
    cachedDistances = QHash<QPair<QGeoCoordinate,QGeoCoordinate>, double>();
    currentRadius = 0.0;
}


Context::Context(Quad *placesQuad, Place * newOrigin
                 , double radius, enum Comparisons::Metric metric) : Context()
{
    update(placesQuad, newOrigin, radius, metric);
}


void Context::update(Quad *placesQuad, Place *newOrigin
                     , double radius, enum Comparisons::Metric metric)
{
    polarCoordinates.clear();
    emit(clearScreen());
    emit(changeRadius(radius));

   // Empty list of polars if the origin has changed because
   // now distances  are different.
   // If the required radius is smaller, clear the list as well because
   // it contains places that are not requested.
   if (newOrigin != origin || currentRadius < radius)
   {
       origin = newOrigin;
   } // if
   currentRadius = radius; // TODO : Check for previous radius.

   // Fetch places relevant to the required distance from the data structure.
   // Range of search is a bit bigger than required.

   QGeoCoordinate originPoint = origin->coord;

   QGeoCoordinate bottomLeftBound = originPoint.atDistanceAndAzimuth(radius*1000, 270).atDistanceAndAzimuth(radius*1000,180);
   QGeoCoordinate topRightBound = originPoint.atDistanceAndAzimuth(radius*1000, 90).atDistanceAndAzimuth(radius*1000,0);

   if (bottomLeftBound.latitude() < placesQuad->bottomLeftPoint.latitude()) {
        bottomLeftBound.setLatitude( placesQuad->bottomLeftPoint.latitude());
   }

   if (bottomLeftBound.longitude() < placesQuad->bottomLeftPoint.longitude()) {
       bottomLeftBound.setLongitude(placesQuad->bottomLeftPoint.longitude());
   }

   if(topRightBound.latitude() > placesQuad->topRightPoint.latitude()) {
       topRightBound.setLatitude(placesQuad->topRightPoint.latitude());
   }
   if (topRightBound.longitude() > placesQuad->topRightPoint.longitude()) {
       topRightBound.setLongitude(placesQuad->topRightPoint.longitude());
   }

   qDebug() << radius;

   qDebug() << "Bottom Left: " << bottomLeftBound.latitude() << ", " << bottomLeftBound.longitude();
   qDebug() << "Top Right: " << topRightBound.latitude() << ", " << topRightBound.longitude();

   qDebug() << "Big Bound TR: " << placesQuad->topRightPoint.latitude() << ", " <<
               placesQuad->topRightPoint.longitude();

   qDebug() << "Big Bound BL: " << placesQuad->bottomLeftPoint.latitude() << ", " <<
               placesQuad->bottomLeftPoint.longitude();

   QSet<Place*> relevantPlaces = placesQuad->search(bottomLeftBound, topRightBound);

   qDebug() << "Diagonal search region: " << bottomLeftBound.distanceTo(topRightBound) / 1000;

   QList <Place*> placesToSearch = QList<Place*>();

   // Look in cache if distance was previously computed.

   Polar polarCoordinate;
   foreach (Place * placePtr, relevantPlaces)
   {
       QPair<QGeoCoordinate, QGeoCoordinate> fromToPair(originPoint, placePtr->coord);
       //if (cachedDistances.contains(fromToPair))
       //{
       //   polarCoordinate.place = placePtr;
       //   polarCoordinate.distance = cachedDistances[fromToPair];
       //   polarCoordinates.append(polarCoordinate);
       //} // if
       //else
           placesToSearch.append(placePtr);
   } //  foreach

   // Get new accureate distances from maps API.
   Comparisons  comparisons = Comparisons();

   comparisons.metric = metric;
   QList<double> searchResults = comparisons.graphDistence(newOrigin, placesToSearch);

   emit(splat(origin->classType == Place::HlthFac ? "hosp" : "school", 0, 0));

   Place::Type allowed = (origin->classType == Place::HlthFac ? Place::Schl : Place::HlthFac);

   Place * placePtr;
   for (int index = 0; index < searchResults.length(); index++)
   {
       double result = searchResults[index];
       placePtr = placesToSearch[index];

       if (placePtr->classType == allowed) {
       // Cache new distances.
       QPair<QGeoCoordinate, QGeoCoordinate>
               fromToPair(originPoint, placePtr->coord);
       //cachedDistances[fromToPair] = result;

       // Add new place to member list.
       polarCoordinate.place = placePtr;
       polarCoordinate.distance = result;
       polarCoordinate.angle =
               originPoint.azimuthTo(placePtr->coord);

       polarCoordinates.append(polarCoordinate);
       emit(splat(polarCoordinate.place->classType == Place::HlthFac ? "hosp" : "school", polarCoordinate.angle, polarCoordinate.distance));
       }
   } // foreach
} // update

// TODO:
// clean cache

