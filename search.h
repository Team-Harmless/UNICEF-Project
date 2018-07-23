#ifndef SEARCH_H
#define SEARCH_H
#include <QString>
#include <QList>
#include "place.h"

/*
 * Assumtions:
 * Both Data and Data->data are heap allocated
*/


struct Tre {
    Place *data;
    struct Tre *next[26];
};

class Search
{
public:
    Search();
    ~Search();
    void addData(Place *data);
    void addData(QList<Place*> places);
    Tre* searchForTre(QString);
    QList<Place*> searchForList(QString);
    Place* getElement(QString name);

private:

    struct Tre *searchableData;
};

#endif // SEARCH_H
