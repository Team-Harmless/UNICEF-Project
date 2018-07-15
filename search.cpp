#include "search.h"
#include <string.h>
#include <stdlib.h>

Tre* _addSearchableData(Tre *root, char *name, Data *data) {
    if (*name == '\0') {
        root->data = data;
        return root;
    }
    while (*name - (unsigned char)'a' < 0 || *name - (unsigned char)'a' >= 26) name++; // Ignore non-ascii
    unsigned char index = *name - (unsigned char)'a';
    if (root->next[index] == NULL) {
        root->next[index] = (Tre*)malloc(sizeof(Tre));
    }
    return _addSearchableData(root->next[index], name + 1, data);
}

Tre* addSearchableData(Tre *root, char *name, Data *data) {
    int length = strlen(name);
    int i = 0; while (name[i] != ' ' && i < length) i++;
    if (name[i] == ' ') addSearchableData(root, name + i + 1, data); //seporates by space (kinda).
    return _addSearchableData(root, name, data);
}

Tre* _searchFor(Tre *root, char *name) {
    if (*name == '\0') return root;
    while (*name - (unsigned char)'a' < 0 || *name - (unsigned char)'a' >= 26) name++; // Ignore non-ascii
    unsigned char index = *name - (unsigned char)'a';
    if (root->next[index] == NULL) return NULL;
    return _searchFor(root->next[index], name + 1);
}

QList<QString> flattern(Tre *root) {
    QList<QString> data;
    data << *(QString*)root;
    for(int i = 0; i < 26; i++)
        if (root->next[i] != NULL)
            data += flattern(root->next[i]);
    return data;
}

void deleteTre(Tre *root) { //This function is savage. Don't use it unless you know no subtrees are needed.
    for(int i = 0; i < 26; i++)
        if (root->next[i] != NULL) deleteTre(root->next[i]);
    free(root->data->data);
    free(root->data);
    free(root);
}

Search::Search()
{
    searchableData = (Tre*)malloc(sizeof(Tre));
}

Search::~Search()
{
    deleteTre(searchableData);
}

void Search::addData(QString name, Data *data)
{
    Tre *inserted = addSearchableData(searchableData, name.toLower().toLatin1().data(), data);
    inserted->data->name = &name;
}

Tre* Search::searchForTre(QString name)
{
    return _searchFor(searchableData, name.toLower().toLatin1().data());
}

QList<QString> Search::searchForList(QString name)
{
    Tre *results = searchForTre(name);
    if (results == NULL) QList<QString>();
    return flattern(results);
}

Data* Search::getElement(QString name)
{
    Tre *ret = _searchFor(searchableData, name.toLower().toLatin1().data());
    if (ret == NULL) return NULL;
    return (Data*)ret;
}
