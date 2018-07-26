#include "context.h"

Context::Context()
{
    placesQuad = nullptr;
    polarCoordinates = QList<Polar>();
    cachedDistances = QHash<QPair<QGeoCoordinate,QGeoCoordinate>, double>();
}


Context::Context(Quad *placesQuad, Place * origin
                 , double radius, enum Comparisons::Metric metric) : Context()
{
    update(placesQuad, origin, radius, metric);
}


void Context::update(Quad *placesQuad, Place *origin
                     , double radius, enum Comparisons::Metric metric)
{
   currentRadius = radius; // TODO : Check for previous radius.
   QGeoCoordinate originPoint = origin->coord;

   double latitudeDeviation = radius / kmPerLatitude;
   double longitudeDeviation = radius / kmPerLongitude;

   QGeoCoordinate bottomLeftBound(originPoint.latitude() - latitudeDeviation
                                  , originPoint.longitude() - longitudeDeviation);
   QGeoCoordinate topRightBound(originPoint.latitude() + latitudeDeviation
                                  , originPoint.longitude() + longitudeDeviation);

   QSet<Place*> relevantPlaces = placesQuad->search(bottomLeftBound, topRightBound);

   QList <QGeoCoordinate> placesToSearch = QList<QGeoCoordinate>();

   Polar polarCoordinate;
   foreach (Place * placePtr, relevantPlaces)
   {
       QPair<QGeoCoordinate, QGeoCoordinate> fromToPair(originPoint, placePtr->coord);
       if (cachedDistances.contains(fromToPair))
       {
          polarCoordinate.place = placePtr;
          polarCoordinate.distance = cachedDistances[fromToPair];
          polarCoordinates.append(polarCoordinate);
       } // if
       else
           placesToSearch.append(placePtr->coord);
   } //  foreach

   // Get accurate distances from maps API.
   Comparisons  comparisons = Comparisons();

   comparisons.metric = metric;
   QList<double> searchResults = comparisons.graphDistence(origin->coord, placesToSearch);

   for (int index = 0; index <
   foreach (double result, searchResults)
   {
       polarCoordinate.distance = result;
       // TODO : fix
       polarCoordinate.angle = originPoint.azimuthTo(placePtr->coord);

       // Cache result.
       QPair<QGeoCoordinate, QGeoCoordinate>
               fromToPair(originPoint, placePtr->coord);
       cachedDistances[fromToPair] = result;
   } // foreach


}



