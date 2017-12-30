#include "component.h"

component::component()
{
    belongingPixels = new QVector<QPoint>();
    boundaryPixels = new QVector<QPoint>();
}

component::component(int level) : grayscaleLevel(level) {
    belongingPixels = new QVector<QPoint>();
    boundaryPixels = new QVector<QPoint>(); // C++03 does not support delegated constructors :(
}

void component::addPoint(QPoint* point) {
    belongingPixels->push_back(*point);
}

void component::addPoint(int x, int y) {
    QPoint* newPoint = new QPoint(x,y);
    addPoint(newPoint);
}

void component::calculateBoundary(neighborhoodType neighborhood=EIGHT) {
    foreach (QPoint p, *(this->belongingPixels)) {
        QVector<QPoint> neighbors = ApoImgLoaders::createNeighborhood (p.rx(), p.ry(), 100000, 100000, neighborhood);
        foreach (QPoint n, neighbors) {
            if (!this->belongingPixels->contains(n)) {this->boundaryPixels->push_back(p); break;}
        }
    }
}

void component::printComponent(displayType how=COORDINATES, int maxX=0, int maxY=0) { // if displayType is set to COORDINATES, it will print the (x,y) tuples of pixels beloging to this component.
                                                                         // if it is set to BITMAP, it will print 1 if a given pixel belongs to the component and 0 otherwise. Bitmap size has to be passed explicitly as maxX and maxY parameters.
    if (how==COORDINATES) {
        QString tuplesList;
        for (int i=0; i<belongingPixels->size(); i++) {
            QString thisTuple;
            QPoint herePoint = belongingPixels->at(i);
            thisTuple.sprintf("[%d,%d] ", herePoint.rx(), herePoint.ry());
            tuplesList.append(thisTuple);
        }
        qDebug() << "Component coordinates at GL = " << grayscaleLevel << ": " << tuplesList;
    }
    else if (how==BOUNDARY){
        QString tuplesList;
        for (int i=0; i<boundaryPixels->size(); i++) {
            QString thisTuple;
            QPoint herePoint = boundaryPixels->at(i);
            thisTuple.sprintf("[%d,%d] ", herePoint.rx(), herePoint.ry());
            tuplesList.append(thisTuple);
        }
        qDebug() << "Boundary coordinates at GL = " << grayscaleLevel << ": " << tuplesList;
    }
}

component component::mergeWith(component* other) {
    component resulting;
    foreach (QPoint p, *(this->belongingPixels)) resulting.addPoint(&p);
    foreach (QPoint p, *(other->belongingPixels)) resulting.addPoint(&p);
    return resulting;
}

void component::setSuccessorComponent(component *other) {
    this->componentUpInTheTreeHierarchy=other;
}
