#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <iterator>
#include <QDebug>
#include <QString>
#include "extractdata.h"
#include "school.h"
#include "healthfacility.h"

QList<Place*> Extractor::getSchools(QString filepath)
{
    QFile sourceFile;
    sourceFile.setFileName(filepath);
    sourceFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString sourceData = sourceFile.readAll();
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(sourceData.toUtf8(), &error);
    QJsonObject sourceAsJson = jsonDocument.object();
    QJsonArray jsonArray = sourceAsJson["features"].toArray();

    //*arraySize = jsonArray.size(); keeping because gonna change to qvector //TODO
    QList<Place*> ret;
    for (int schoolCount = 0; schoolCount < jsonArray.size(); schoolCount++)
    {
        QJsonObject schoolObject = jsonArray.at(schoolCount).toObject();
        QJsonObject properties = schoolObject["properties"].toObject();
        School *school = new School();
        school->classType = Place::Schl;
        school->name = properties["name"].toString();
        school->id = properties["admin_id"].toString();
        QJsonArray colourArray = properties["color"].toArray();

        for (int index = 0; index < colourArray.size(); index++)
            school->colour[index] = colourArray.at(index).toInt();

        QJsonObject geometry = schoolObject["geometry"].toObject();
        QJsonArray  coordinates = geometry["coordinates"].toArray();
        school->xCoordinate = coordinates.takeAt(0).toDouble();
        school->yCoordinate = coordinates.takeAt(1).toDouble();
        ret << school;
    }
    return ret;
} 

QList<Place*> Extractor::getHealthFacilities(QString filepath)
{
    QFile sourceFile;
    sourceFile.setFileName(filepath);
    sourceFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString sourceData = sourceFile.readAll();
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(sourceData.toUtf8(), &error);
    QJsonObject sourceAsJson = jsonDocument.object();
    QJsonArray jsonArray = sourceAsJson["features"].toArray();

    //*arraySize = jsonArray.size(); keeping becuase gonna change to qvector //TODO
    QList<Place*> ret;
    for (int healthFacilityCount = 0; healthFacilityCount < jsonArray.size(); healthFacilityCount++)
    {
        QJsonObject healthFacilityObject = jsonArray.at(healthFacilityCount).toObject();
        QJsonObject properties = healthFacilityObject["properties"].toObject();
        HealthFacility *healthFacility = new HealthFacility();
        healthFacility->classType = Place::HlthFac;
        healthFacility->id = properties["uuid"].toString();
        healthFacility->name = properties["name"].toString();
        healthFacility->type = properties["type"].toString();
        healthFacility->dateModified = properties["date_modified"].toString();
        healthFacility->sourceURL = properties["source_url"].toString();
        healthFacility->source = properties["source"].toString();
        healthFacility->completeness = properties["completeness"].toString();
        healthFacility->version = properties["version"].toString();
        healthFacility->upstream = properties["upstream"].toString();
        healthFacility->what3words = properties["what3words"].toString();
        QJsonObject geometry = properties["geometry"].toObject();
        QJsonArray  coordinates  = properties["geometry"].toArray();
        healthFacility->xCoordinate = coordinates.at(0).toDouble();
        healthFacility->yCoordinate = coordinates.at(1).toDouble();
        ret << healthFacility;
    }
    return ret;
}


//    School * schools;
//    int schoolsArraySize;
//    getSchools(schools, &schoolsArraySize);
//    qDebug() << schools[0].colour[0];
//    HealthFacility * healthFacilities;
//    int healthFacilitiesArraySize;
//    getHealthFacilities(healthFacilities, &healthFacilitiesArraySize);
//    qDebug() << healthFacilities[0].id;
