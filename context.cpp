#include "context.h"

Context::Context()
{
    placesQuad = nullptr;
    placesList = QList<Place *>();
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
}



