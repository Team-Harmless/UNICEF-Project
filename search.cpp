#include "search.h"
#include <string.h>
#include <stdlib.h>
#include <QDebug>


Tre* _addSearchableData(Tre *root, char *name, Place *data) {
    if (*name == '\0') {
        root->data = data;
        return root;
    }
    while (*name - (unsigned char)'a' < 0 || *name - (unsigned char)'a' >= 26) name++; // Ignore non-ascii
    unsigned char index = *name - (unsigned char)'a';
    if (root->next[index] == NULL) {
        root->next[index] = (Tre*)malloc(sizeof(Tre));
        for(int i = 0; i < 26; i++)
            root->next[index]->next[i] = NULL;
        root->next[index]->data = NULL;
    }
    return _addSearchableData(root->next[index], name + 1, data);
}

Tre* addSearchableData(Tre *root, char *name, Place *data) {
    int length = strlen(name);
    int i = 0; while (name[i] != ' ' && i < length) i++;
    if (name[i] == ' ') addSearchableData(root, name + i + 1, data); //seporates by space (kinda).
    return _addSearchableData(root, name, data);
}

Tre* _searchFor(Tre *root, char *name) {
    #ifdef DEEP_DEBUG
    qDebug() << "Called " << __FUNCTION__
             << "( root," << name << ")"
             << ":: Length of name = "
             << strlen(name);
    #endif
    if (*name == '\0') return root;
    while (*name - (unsigned char)'a' < 0 || *name - (unsigned char)'a' > 25) name++; // Ignore non-ascii
    unsigned char index = *name - (unsigned char)'a';
    if (root->next[index] == NULL) return NULL;
    return _searchFor(root->next[index], name + 1);
}

QList<Place*> flattern(Tre *root) {
#ifdef DEEP_DEBUG
qDebug() << "Called " << __FUNCTION__ << "(" << (root == NULL ? "NULL" : "root") << ")";
#endif
    QList<Place*> data;
    if (root->data != NULL) data << root->data;
    for(int i = 0; i < 26; i++)
        if (root->next[i] != NULL)
            data += flattern(root->next[i]);
    return data;
}

void deleteTre(Tre *root) { //This function is savage. Don't use it unless you know no subtrees are needed.
    for(int i = 0; i < 26; i++)
        if (root->next[i] != NULL) deleteTre(root->next[i]);
    free(root);
}

int count(Tre *root) {
    if (root == NULL) return 0;
    int counter = 1;
    for(int i = 0; i < 26; i++)
        if (root->next[i] != NULL) counter += count(root->next[i]);
    return counter;
}

Search::Search()
{
    searchableData = (Tre*)malloc(sizeof(Tre));
    searchableData->data = NULL;
    for (int i = 0; i < 26; i++) {
        searchableData->next[i] = NULL;
    }
}

Search::~Search()
{
    deleteTre(searchableData);
}

void Search::addData(Place *data)
{
    addSearchableData(searchableData, data->name.toLower().toLatin1().data(), data);
}

void Search::addData(QList<Place*> places)
{
    foreach (Place *pl, places) {
        addData(pl);
    }
}

Tre* Search::searchForTre(QString name)
{
    return _searchFor(searchableData, name.toLower().toLatin1().data());
}

QList<Place*> Search::searchForList(QString name)
{
    Tre *results = searchForTre(name);
    if (results == NULL) return QList<Place*>();
    return flattern(results);
}

Place* Search::getElement(QString name)
{
    Tre *ret = _searchFor(searchableData, name.toLower().toLatin1().data());
    if (ret == NULL) return NULL;
    return (Place*)ret;
}
