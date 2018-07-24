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
        timeDistance = 2,
    };
    Metric metric;
    double graphDistence(QGeoCoordinate pointA, QGeoCoordinate pointB);
    QList<double> graphDistence(QGeoCoordinate pointA, QList<QGeoCoordinate> pointB);
    QString bingMapsAPIKey;

private:
    double strightLineMethod(QGeoCoordinate pointA, QGeoCoordinate pointB);
    double roadMethod(QGeoCoordinate pointA, QGeoCoordinate pointB);
    double timeMethod(QGeoCoordinate pointA, QGeoCoordinate pointB);

    QList<double> strightLineMethod(QGeoCoordinate pointA, QList<QGeoCoordinate> pointB);
    QList<double> roadMethod(QGeoCoordinate pointA, QList<QGeoCoordinate> pointB);
    QList<double> timeMethod(QGeoCoordinate pointA, QList<QGeoCoordinate> pointB);
};

#endif // COMPARISONS_H
