#ifndef SCHOOLIMPORT_H
#define SCHOOLIMPORT_H
#include <QString>


class SchoolImporter
{
public:
    SchoolImporter();
    void setSourceFile(const QString file);
    void beginIndexing();

private:
    QString jsonFile;
};

#endif // SCHOOLIMPORT_H
