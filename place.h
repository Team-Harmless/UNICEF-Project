#ifndef PLACE_H
#define PLACE_H

#include <QString>
#include <QGeoCoordinate>

class Place
{
public:
    Place();
    QString name;
    QString id;
    QGeoCoordinate coordinate;
};

#endif // PLACE_H
