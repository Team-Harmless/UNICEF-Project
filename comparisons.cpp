#include "comparisons.h"

Comparisons::Comparisons()
{

}

double Comparisons::graphDistence(QGeoCoordinate pointA, QGeoCoordinate pointB)
{
    switch (metric) {
        case StrightLineDistance:
            return strightLineMethod(pointA, pointB);
        case RoadDistance:
            return roadMethod(pointA, pointB);
        case TrafficDistance:
            return 0;
        default:
            return 0;
    }
}

double Comparisons::roadMethod(QGeoCoordinate pointA, QGeoCoordinate pointB) {

}

double Comparisons::strightLineMethod(QGeoCoordinate pointA, QGeoCoordinate pointB) {
    return pointA.distanceTo(pointB);
}

double Comparisons::trafficMethod(QGeoCoordinate pointA, QGeoCoordinate pointB) {

}
