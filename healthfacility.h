#ifndef HEALTHFACILITY_H
#define HEALTHFACILITY_H


class HealthFacility : public Place
{
public:
    HealthFacility();
    QString type;
    QString source;
    QString sourceURL;
    QString dateModified;
    QString completeness;
    QString version;
    QString upstream;
    QString what3words;
};

#endif // HEALTHFACILITY_H
