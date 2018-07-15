#include "search.h"
#include <string.h>
#include <stdlib.h>

struct tre {
    void *data;
    struct tre *next[26];
};

typedef struct tre Tre;

void _addSearchableData(Tre *root, char *name, void *data) {
    if (*name = '\0') {
        root->data = data;
        return;
    }
    unsigned char index = *name - (unsigned char)'a';
    if (root->next[index] == NULL)
        root->next[index] = (Tre*)malloc(sizeof(Tre));
    _addSearchableData(root->next[index], name + 1, data);
}

void addSearchableData(Tre *root, char *name, void *data) {
    int length = strlen(name);
    int i = 0; while (name[i] != ' ' && i < length) i++;
    if (name[i] == ' ') addSearchableData(root, name + i + 1, data);
    _addSearchableData(root, name, data);
}

Tre *searchableIndex = NULL;

Search::Search()
{
    searchableIndex = (Tre*)malloc(sizeof(Tre));
}
