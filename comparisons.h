#ifndef COMPARISONS_H
#define COMPARISONS_H

#include <QGeoCoordinate>
#include <QString>

class Comparisons
{
public:
    Comparisons();
    enum Metric{
        StrightLineDistance = 0,
        RoadDistance = 1,
        TrafficDistance = 2,
    };
    Metric metric;
    double graphDistence(QGeoCoordinate pointA, QGeoCoordinate pointB);
    QString bingMapsAPIKey;

private:
    double strightLineMethod(QGeoCoordinate pointA, QGeoCoordinate pointB);
    double roadMethod(QGeoCoordinate pointA, QGeoCoordinate pointB);
    double trafficMethod(QGeoCoordinate pointA, QGeoCoordinate pointB);
};

#endif // COMPARISONS_H
