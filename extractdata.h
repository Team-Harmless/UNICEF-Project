#ifndef EXTARCTDATA_H
#define EXTARCTDATA_H

#endif // EXTARCTDATA_H

#include <QList>
#include "place.h"
class Extractor {
public:
    static QList<Place*> getSchools(QString filepath);
    static QList<Place*> getHealthFacilities(QString filepath);
};
