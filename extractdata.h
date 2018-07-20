#ifndef EXTARCTDATA_H
#define EXTARCTDATA_H

#endif // EXTARCTDATA_H

#include <QList>

struct School
{
    QString id;
    int colour[3];
    QString name;
    double xCoordinate;
    double yCoordinate;

};

struct HealthFacility
{
  QString name;
  QString type;
  QString source;
  QString sourceURL;
  QString dateModified;
  QString id;
  QString completeness;
  QString version;
  QString upstream;
  QString what3words;
  double xCoordinate;
  double yCoordinate;
};

QList<School> getSchools(QString filepath);
QList<HealthFacility> getHealthFacilities(QString filepath);
School* getSchools();


