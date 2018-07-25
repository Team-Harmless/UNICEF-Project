#ifndef CONTEXT_H
#define CONTEXT_H

#include <QHash>
#include <QPair>

#include <quadtree.h>
#include <place.h>

struct Polar
{
    Place * place;
    double distance;
    double angle;
};

class Context
{
public:
    Context();
    Context(Quad *placesQuad, Place * origin, double radius);
    void update(Quad *placesQuad, Place * origin, double radius);
    QList<Polar> polarCoordinates;
private:
    Quad *placesQuad;
    QHash<QPair<QGeoCoordinate,QGeoCoordinate>, double> cachedDistances;

    double kmPerLatitude = 100; // actually average is 111
    double kmPerLongitude = 10; // actually between 19 and 43

};

#endif // CONTEXT_H
