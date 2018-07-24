#include "comparisons.h"
#include <QNetworkAccessManager>
#include <QUrl>
#include <QUrlQuery>

QString coordToStr(QGeoCoordinate c) {
    double pointALat  = c.latitude();
    double pointALong = c.longitude();

    return QString::number(pointALat) + "," + QString::number(pointALong);
}

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
        case timeDistance:
            return timeMethod(pointA, pointB);
        default:
            return 0;
    }
}

QList<double> Comparisons::graphDistence(QGeoCoordinate pointA, QList<QGeoCoordinate> pointB)
{
    switch (metric) {
        case StrightLineDistance:
            return strightLineMethod(pointA, pointB);
        case RoadDistance:
            return roadMethod(pointA, pointB);
        case timeDistance:
            return timeMethod(pointA, pointB);
        default:
            return QList<double>();
    }
}


double Comparisons::roadMethod(QGeoCoordinate pointA, QGeoCoordinate pointB) {
    QUrl url ("https://dev.virtualearth.net/REST/v1/Routes/DistanceMatrix");
    QUrlQuery query;

    query.addQueryItem("travelMode","driving");
    query.addQueryItem("origins", coordToStr(pointA));
    query.addQueryItem("destinations", coordToStr(pointB));
    query.addQueryItem("key", bingMapsAPIKey);
    url.setQuery(query);

    QNetworkAccessManager man;
    QNetworkRequest request(url);
    QNetworkReply *reply = man.get(request);
    return 0.0;
}

double Comparisons::timeMethod(QGeoCoordinate pointA, QGeoCoordinate pointB) {
    QUrl url ("https://dev.virtualearth.net/REST/v1/Routes/DistanceMatrix");
    QUrlQuery query;

    query.addQueryItem("travelMode","driving");
    query.addQueryItem("origins", coordToStr(pointA));
    query.addQueryItem("destinations", coordToStr(pointB));
    query.addQueryItem("key", bingMapsAPIKey);
    url.setQuery(query);

    QNetworkAccessManager man;
    QNetworkRequest request(url);
    QNetworkReply *reply = man.get(request);
    return 0.0;
}

double Comparisons::strightLineMethod(QGeoCoordinate pointA, QGeoCoordinate pointB) {
    return pointA.distanceTo(pointB) / 1000;
}

QList<double> Comparisons::strightLineMethod(QGeoCoordinate pointA, QList<QGeoCoordinate> pointB) {
    QList<double> ret;
    foreach (QGeoCoordinate point, pointB)
        ret << pointA.distanceTo(point) / 1000;
    return ret;
}

QList<double> Comparisons::roadMethod(QGeoCoordinate pointA, QList<QGeoCoordinate> pointB) {
    QUrl url ("https://dev.virtualearth.net/REST/v1/Routes/DistanceMatrix");
    QUrlQuery query;

    query.addQueryItem("travelMode","driving");
    query.addQueryItem("origins", coordToStr(pointA));
    QString destinations;
    foreach(QGeoCoordinate point, pointB) {
        destinations += coordToStr(point) + ";";
    }
    destinations = destinations.left(destinations.lastIndexOf(';'));
    query.addQueryItem("destinations", destinations);
    query.addQueryItem("key", bingMapsAPIKey);
    url.setQuery(query);

    QNetworkAccessManager man;
    QNetworkRequest request(url);
    QNetworkReply *reply = man.get(request);
    return QList<double>();
}

QList<double> Comparisons::timeMethod(QGeoCoordinate pointA, QList<QGeoCoordinate> pointB) {
    QUrl url ("https://dev.virtualearth.net/REST/v1/Routes/DistanceMatrix");
    QUrlQuery query;

    query.addQueryItem("travelMode","driving");
    query.addQueryItem("origins", coordToStr(pointA));
    QString destinations;
    foreach(QGeoCoordinate point, pointB) {
        destinations += coordToStr(point) + ";";
    }
    destinations = destinations.left(destinations.lastIndexOf(';'));
    query.addQueryItem("destinations", destinations);
    query.addQueryItem("key", bingMapsAPIKey);
    url.setQuery(query);

    QNetworkAccessManager man;
    QNetworkRequest request(url);
    QNetworkReply *reply = man.get(request);
    return QList<double>();
}
