#ifndef SEARCH_H
#define SEARCH_H
#include <QString>
#include <QList>

/*
 * Assumtions:
 * Both Data and Data->data are heap allocated
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
    void addData(Data *data);
    Tre* searchForTre(QString);
    QList<QString> searchForList(QString);
    Data* getElement(QString name);

private:

    struct Tre *searchableData;
};

#endif // SEARCH_H
