#include "context.h"

Context::Context()
{
    placesQuad = nullptr;
    polarCoordinates = QList<Polar>();
    cachedDistances = QHash<QPair<QGeoCoordinate,QGeoCoordinate>, double>();
}


Context::Context(Quad *placesQuad, Place * origin, double radius) : Context()
{
    update(placesQuad,origin,radius);
}


void Context::update(Quad *placesQuad, Place *origin, double radius)
{
   QGeoCoordinate originPoint = origin->coord;

   double latitudeDeviation = radius / kmPerLatitude;
   double longitudeDeviation = radius / kmPerLongitude;

   QGeoCoordinate bottomLeftBound(originPoint.latitude() - latitudeDeviation
                                  , originPoint.longitude() - longitudeDeviation);
   QGeoCoordinate topRightBound(originPoint.latitude() + latitudeDeviation
                                  , originPoint.longitude() + longitudeDeviation);

   QSet<Place*> relevantPlaces = placesQuad->search(bottomLeftBound, topRightBound);

   QList <QGeoCoordinate> placesToSearch;

   foreach (Place * placePtr, relevantPlaces)
   {
       QPair<QGeoCoordinate, QGeoCoordinate> fromToPair(originPoint, placePtr->coord);
       if (cachedDistances.contains(fromToPair))
       {
          Polar polarCoordinate;
          polarCoordinate.place = placePtr;
          double angle =// QGeo one to another.
          polarCoordinate.angle =
       }

   }
}



