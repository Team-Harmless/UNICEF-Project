#ifndef BROWSERINTEGRATOR_H
#define BROWSERINTEGRATOR_H

#include "place.h"
#include <QUrlQuery>


class browserIntegrator
{
public:
    QUrl getBingUrlOf(Place * place);
    QUrl getBingUrlDistance(Place *origin, Place * destination);
};

#endif // BROWSERINTEGRATOR_H
