#ifndef PLACE_H
#define PLACE_H

#include <QString>

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
    double xCoordinate;
    double yCoordinate;
};

#endif // PLACE_H
