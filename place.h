#ifndef PLACE_H
#define PLACE_H

#include <QString>
#include <QGeoCoordinate>

class Place
{
public:
    enum Type {
        Schl = 1,
        HlthFac = 2,
    };
    Type classType;
    Place();
    QString name;
    QString id;
    QGeoCoordinate coord;
};

#endif // PLACE_H
