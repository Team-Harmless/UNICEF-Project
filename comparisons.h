#ifndef COMPARISONS_H
#define COMPARISONS_H

#include <QGeoCoordinate>
#include <QString>
#include <QJsonDocument>
#include "place.h"
#include <QNetworkReply>
#include <QObject>

class Comparisons : public QObject
{
    Q_OBJECT
public:
    Comparisons();
    enum Metric{
        StrightLineDistance = 0,
        RoadDistance = 1,
        timeDistance = 2,
    };
    Metric metric;
    double graphDistence(QGeoCoordinate pointA, QGeoCoordinate pointB);
    double graphDistence(Place* pointA, Place* pointB);
    QList<double> graphDistence(QGeoCoordinate pointA, QList<QGeoCoordinate> pointB);
    QList<double> graphDistence(Place* pointA, QList<Place*> pointB);
    QString bingMapsAPIKey;

private:
    double strightLineMethod(QGeoCoordinate pointA, QGeoCoordinate pointB);
    double roadMethod(QGeoCoordinate pointA, QGeoCoordinate pointB);
    double timeMethod(QGeoCoordinate pointA, QGeoCoordinate pointB);

    QList<double> strightLineMethod(QGeoCoordinate pointA, QList<QGeoCoordinate> pointB);
    QList<double> roadMethod(QGeoCoordinate pointA, QList<QGeoCoordinate> pointB);
    QList<double> timeMethod(QGeoCoordinate pointA, QList<QGeoCoordinate> pointB);

    QJsonDocument webRequester(QGeoCoordinate pointA, QList<QGeoCoordinate> pointB);

public slots:
    void finishedRequest(QNetworkReply *);
};

#endif // COMPARISONS_H
