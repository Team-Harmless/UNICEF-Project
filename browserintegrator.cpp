#include "browserintegrator.h"

QUrl browserIntegrator::getBingUrlOf(Place *place)
{
    QString placeName = place->name;
    QUrl url = QUrl("http://bing.com/maps/default.aspx");
    QUrlQuery query;
    query.addQueryItem("where1", placeName);
    url.setQuery(query);
    return QUrl;
}
