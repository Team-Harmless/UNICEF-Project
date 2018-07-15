#ifndef SEARCH_H
#define SEARCH_H
#include <QString>
#include <QList>

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
    void addData(QString name, Data *data);
    Tre* searchForTre(QString);
    QList<QString> searchForList(QString);

private:

    struct Tre *searchableData;
};

#endif // SEARCH_H
