#include "context.h"
#include <QDebug>
#include "browserintegrator.h"
#include <QUrl>
#include <QDesktopServices>

QList<Polar> Context::getAllTheJucyData()
{
    return polarCoordinates;
}

void Context::openSingleInBrowser(int arrayIndex)
{
    QUrl url = browserIntegrator::getBingUrlOf(polarCoordinates.at(arrayIndex).place);
    QDesktopServices::openUrl(url);
}

void Context::openRouteInBrowser(int arrayIndex)
{
    QUrl url = browserIntegrator::getBingUrlDistance(origin, polarCoordinates.at(arrayIndex).place);
    QDesktopServices::openUrl(url);
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





   QSet<Place*> relevantPlaces = placesQuad->search(bottomLeftBound, topRightBound);

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

   comparisons.metric = metric;
   comparisons.bingMapsAPIKey = bingAPIKey;
   emit(setMetric(metric));

   Place::Type allowed = (origin->classType == Place::HlthFac ? Place::Schl : Place::HlthFac);

   for(int i = placesToSearch.count() - 1; i >= 0; i--)
       if (placesToSearch.at(i)->classType != allowed || originPoint.distanceTo(placesToSearch.at(i)->coord) / 1000 > radius)
           placesToSearch.removeAt(i);

   QList<double> searchResults = comparisons.graphDistence(newOrigin, placesToSearch);

   Place * placePtr;
   for (int index = 0; index < searchResults.length(); index++)
   {
       double result = searchResults[index];
       placePtr = placesToSearch[index];


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
       emit(splat(polarCoordinate.place->classType == Place::HlthFac ? "hosp" : "school", polarCoordinate.angle, polarCoordinate.distance, placePtr->name, index, true));
       if (polarCoordinate.distance > radius) {
           radius = polarCoordinate.distance + 10;
           emit(changeRadius(radius));
       }

   } // foreach

   emit(splat(origin->classType == Place::HlthFac ? "hosp" : "school", 0, 0, "", 0, false));

}

void Context::updateMult(double mult)
{
    emit(setMultiplier(mult));
}

// TODO:
// clean cache

