#ifndef COMPONENT_H
#define COMPONENT_H
#include <QPoint>
#include <QVector>
#include <QString>
#include <QDebug>
#include "constdefinitions.h"
#include "apoimgloaders.h"

class component
{
public:
    QVector<QPoint>* belongingPixels;
    QVector<QPoint>* boundaryPixels;
    uchar  grayscaleLevel;
    component* componentUpInTheTreeHierarchy;

    component();
    component(int level);
    void addPoint(QPoint* point);                   // these are mutator methods!
    void addPoint(int x, int y);
    void calculateBoundary(neighborhoodType neighborhood);
    void printComponent(displayType dt, int maxX, int maxY);
    component mergeWith(component* other);          // no check is performed on the contiguity of both regions or overlapping pixels
    void setSuccessorComponent(component* other);

#endif // COMPONENT_H
};
