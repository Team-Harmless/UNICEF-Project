#ifndef SEARCH_H
#define SEARCH_H
#include <QString>
#include <QList>

/*
 * Assumtions:
 * Both Data and Data->data are heap allocated
 * Tre *A;
 * (Data*)A is valid
 * (QString*)A is also valid
 * #PointersAreMagic
*/

struct Data {
    QString *name;
    void *data;
};

struct Tre {
    Data *data;
    struct Tre *next[26];
};

class Search
{
public:
    Search();
    ~Search();
    void addData(QString name, Data *data);
    Tre* searchForTre(QString);
    QList<QString> searchForList(QString);
    Data* getElement(QString name);

private:

    struct Tre *searchableData;
};

#endif // SEARCH_H
