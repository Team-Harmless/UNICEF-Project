#include "context.h"

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
   // Empty list of polars if the origin has changed because
   // now distances  are different.
   // If the required radius is smaller, clear the list as well because
   // it contains places that are not requested.
   if (newOrigin != origin || currentRadius < radius)
   {
       origin = newOrigin;
       polarCoordinates = QList<Polar>();
   } // if
   currentRadius = radius; // TODO : Check for previous radius.

   // Fetch places relevant to the required distance from the data structure.
   // Range of search is a bit bigger than required.

   QGeoCoordinate originPoint = origin->coord;

   double latitudeDeviation = radius / kmPerLatitude;
   double longitudeDeviation = radius / kmPerLongitude;

   QGeoCoordinate bottomLeftBound(originPoint.latitude() - latitudeDeviation
                                  , originPoint.longitude() - longitudeDeviation);
   QGeoCoordinate topRightBound(originPoint.latitude() + latitudeDeviation
                                  , originPoint.longitude() + longitudeDeviation);

   QSet<Place*> relevantPlaces = placesQuad->search(bottomLeftBound, topRightBound);


   QList <Place*> placesToSearch = QList<Place*>();

   // Look in cache if distance was previously computed.

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
           placesToSearch.append(placePtr);
   } //  foreach

   // Get new accureate distances from maps API.
   Comparisons  comparisons = Comparisons();

   comparisons.metric = metric;
   QList<double> searchResults = comparisons.graphDistence(origin, placesToSearch);

   Place * placePtr;
   for (int index = 0; index < searchResults.length(); index++)
   {
       double result = searchResults[index];
       placePtr = placesToSearch[index];

       // Cache new distances.
       QPair<QGeoCoordinate, QGeoCoordinate>
               fromToPair(originPoint, placePtr->coord);
       cachedDistances[fromToPair] = result;

       // Add new place to member list.
       polarCoordinate.place = placePtr;
       polarCoordinate.distance = result;
       polarCoordinate.angle =
               originPoint.azimuthTo(placePtr->coord);

       polarCoordinates.append(polarCoordinate);
   } // foreach
} // update

// TODO:
// clean cache

