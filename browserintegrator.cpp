#include "browserintegrator.h"

QUrl browserIntegrator::getBingUrlOf(Place *place)
{
    if (!place)
        return QUrl("");
    QString coordinate = toBingFormat(place->coord);
    QUrl url = QUrl("http://bing.com/maps/default.aspx");
    QUrlQuery query;
    query.addQueryItem("where1", coordinate);
    url.setQuery(query);
    return url;
}

QUrl browserIntegrator::getBingUrlDistance(Place *origin, Place *destination)
{
   QString originCoordinate = toBingFormat(origin->coord);
   QString destinationCoordinate = toBingFormat(destination->coord);

   QString value = "adr." + originCoordinate + "~adr." + destinationCoordinate;

   QUrlQuery query;
   query.addQueryItem("Rtp", value);
   QUrl url = QUrl("http://bing.com/maps/default.aspx");
   url.setQuery(query);
   return url;
}

QString browserIntegrator::toBingFormat(QGeoCoordinate point)
{
    QString result = QString::number(point.latitude());
    result += "," + QString::number(point.longitude());
    return result;
}


// Test
//QList<Place *> schoolsList = Extractor::getSchools("schools.json");
//QUrl url = browserIntegrator::getBingUrlOf(schoolsList[0]);
//QUrl url2 = browserIntegrator::getBingUrlDistance(schoolsList[0], schoolsList[1]);
//qDebug() << url.toString().toLatin1();
//qDebug() << url2;
