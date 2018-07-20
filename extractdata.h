#ifndef EXTARCTDATA_H
#define EXTARCTDATA_H

#endif // EXTARCTDATA_H

#include <QList>
#include <school.h>
#include <healthfacility.h>

QList<School> getSchools(QString filepath);
QList<HealthFacility> getHealthFacilities(QString filepath);
School* getSchools();


