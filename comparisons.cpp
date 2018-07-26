#include "comparisons.h"
#include <QNetworkAccessManager>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>

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
double Comparisons::graphDistence(Place *pointA, Place *pointB)
{
    return graphDistence(pointA->coord, pointB->coord);
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

QList<double> Comparisons::graphDistence(Place *pointA, QList<Place *> pointB)
{
    return graphDistence(pointA->coord, [](QList<Place*> points) -> QList<QGeoCoordinate>
    {QList<QGeoCoordinate> ret; foreach (Place* pl, points) ret << pl->coord; return ret;}(pointB));
}

QJsonDocument Comparisons::webRequester(QGeoCoordinate pointA, QList<QGeoCoordinate> pointB) {
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

    qDebug() << url.toString();

    QNetworkAccessManager man;
    QNetworkRequest request(url);
    QNetworkReply *reply = man.get(request);
    while(reply->isRunning()) ;;
    if(reply->error() != QNetworkReply::NoError) return QJsonDocument();

    return QJsonDocument::fromBinaryData(reply->readAll());
}


double Comparisons::roadMethod(QGeoCoordinate pointA, QGeoCoordinate pointB) {
    QJsonDocument json = webRequester(pointA, {pointB});
    if (json.object().count() == 0) return 0.0;
    QJsonArray results = json.object()["resourceSets"].toArray().at(0)["results"].toArray();
    return results.at(0)["travelDistance"].toDouble();
}

double Comparisons::timeMethod(QGeoCoordinate pointA, QGeoCoordinate pointB) {
    QJsonDocument json = webRequester(pointA, {pointB});
    if (json.object().count() == 0) return 0.0;
    QJsonArray results = json.object()["resourceSets"].toArray().at(0)["results"].toArray();
    return results.at(0)["travelDuration"].toDouble();
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
    QJsonDocument json = webRequester(pointA, pointB);
    QList<double> ret;
    if (json.object().count() == 0) return {0.0};
    QJsonArray results = json.object()["resourceSets"].toArray().at(0)["results"].toArray();
    for(int i = 0; i < results.count(); i++)
        ret << results.at(i)["travelDistance"].toDouble();
    return ret;
}

QList<double> Comparisons::timeMethod(QGeoCoordinate pointA, QList<QGeoCoordinate> pointB) {
    QJsonDocument json = webRequester(pointA, pointB);
    QList<double> ret;
    if (json.object().count() == 0) return {0.0};
    QJsonArray results = json.object()["resourceSets"].toArray().at(0)["results"].toArray();
    for(int i = 0; i < results.count(); i++)
        ret << results.at(i)["travelDuration"].toDouble();
    return ret;
}
