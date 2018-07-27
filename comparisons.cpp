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
#include <QEventLoop>
#include <QTimer>
#include <QJsonParseError>

QString coordToStr(QGeoCoordinate c) {
    double pointALat  = c.latitude();
    double pointALong = c.longitude();

    return QString::number(pointALat) + "," + QString::number(pointALong);
}

bool finishedRequestBool = false;

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

    //qDebug() << url.toString();

    QNetworkAccessManager *man = new QNetworkAccessManager(this);

    QNetworkRequest *request = new QNetworkRequest(url);
    QTimer timer;
    timer.setSingleShot(true);
    QNetworkReply *reply = man->get(*request);
    QEventLoop eventLoop;
    QObject::connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    timer.start(30000);
    eventLoop.exec();
    if(reply->error() != QNetworkReply::NoError) {qDebug() << "ERROR"; return QJsonDocument();}

    //qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    QJsonParseError jpe;
    QJsonDocument ret = QJsonDocument::fromJson(reply->readAll(), &jpe);
    //qDebug() << jpe.errorString() << " at " << jpe.offset << " : ";
    delete man;
    delete request;
    return ret;
}

void Comparisons::finishedRequest(QNetworkReply*)
{
    finishedRequestBool = true;
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
    QJsonArray results = json.object()["resourceSets"].toArray().at(0)
                                      ["resources"].toArray().at(0)
                                      ["results"].toArray();
    //qDebug() << results;
    for(int i = 0; i < results.count(); i++)
        ret << results.at(i)["travelDistance"].toDouble();
    return ret;
}

QList<double> Comparisons::timeMethod(QGeoCoordinate pointA, QList<QGeoCoordinate> pointB) {
    QJsonDocument json = webRequester(pointA, pointB);
    QList<double> ret;
    if (json.object().count() == 0) return {0.0};
    QJsonArray results = json.object()["resourceSets"].toArray().at(0)
                                      ["resources"].toArray().at(0)
                                      ["results"].toArray();
    for(int i = 0; i < results.count(); i++)
        ret << results.at(i)["travelDuration"].toDouble();
    return ret;
}
