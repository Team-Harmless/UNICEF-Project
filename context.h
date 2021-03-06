#ifndef CONTEXT_H
#define CONTEXT_H

#include <QHash>
#include <QPair>

#include <quadtree.h>
#include <place.h>
#include <comparisons.h>
#include <QObject>

// Contains the place with its polar coordinates.
struct Polar
{
    Place * place;
    double distance;
    double angle;
};

class Context : public QObject {
        Q_OBJECT
public:
    Q_INVOKABLE QList<Polar>getAllTheJucyData();

    Q_INVOKABLE void openSingleInBrowser(int arrayIndex);
     Q_INVOKABLE void openRouteInBrowser(int arrayIndex);

    Context(); // Initiate object.

    // Create object and do a search.
    Context(Quad *placesQuad, Place * newOrigin
            , double radius, enum Comparisons::Metric metric);

    // Do a search. It updates public members.
    void update(Quad *placesQuad, Place * newOrigin
                , double radius, enum Comparisons::Metric metric);


    QList<Polar> polarCoordinates; // Contains points for the UI.
    double currentRadius; // Keep track of the requested radius.

    void updateMult(double mult);
    QString bingAPIKey;

    Comparisons comparisons;
signals:
    void splat(QString type, double angle, double distance, QString name, int index = 0, bool clickable = false);
    void clearScreen();
    void changeRadius(double rad);
    void setMultiplier(double mult);
    void setMetric(int met);

private:
    Place * origin; // If origin changes polar list is refreshed.

    // Cache results in order to not recompute them every time.
    QHash<QPair<QGeoCoordinate,QGeoCoordinate>, double> cachedDistances;

};

#endif // CONTEXT_H
