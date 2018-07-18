#include <qfile.h>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <iterator>
#include <QDebug>
#include <QString>

struct School
{
    QString id;
    int colour[3];
    QString name;
    double xCoordinate;
    double yCoordinate;

};

School * getData()
{
    QFile sourceFile;
    sourceFile.setFileName("schools.json");
    sourceFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString sourceData = sourceFile.readAll();
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(sourceData.toUtf8(), &error);
    QJsonObject sourceAsJson = jsonDocument.object();
    QJsonArray jsonArray = sourceAsJson["features"].toArray();

    School * schools = new School[jsonArray.size()];
    int schoolCount = 0;
    foreach (const QJsonValue & value, jsonArray)
    {
        QJsonObject schoolObject = value.toObject();
        QJsonObject properties = schoolObject["properties"].toObject();
        struct School school;
        school.id = properties["admin_id"].toString();
        QJsonArray colourArray = properties["color"].toArray();

        int index = 0;
        foreach (const QJsonValue & colour, colourArray)
        {
            school.colour[index] = colour.toInt();
            index++;
        }

        QJsonObject geometry = schoolObject["geometry"].toObject();
        QJsonArray  coordinates = geometry["coordinates"].toArray();
        school.xCoordinate = coordinates.takeAt(0).toDouble();
        school.yCoordinate = coordinates.takeAt(1).toDouble();
        schools[schoolCount] = school;
        schoolCount++;
    }
    return schools;
}
