#ifndef BROWSERINTEGRATOR_H
#define BROWSERINTEGRATOR_H

#include "place.h"
#include <QUrlQuery>


class browserIntegrator
{
public:
    static QUrl getBingUrlOf(Place * place);
    static QUrl getBingUrlDistance(Place *origin, Place * destination);

private:
    static QString toBingFormat(QGeoCoordinate point);
};

#endif // BROWSERINTEGRATOR_H
