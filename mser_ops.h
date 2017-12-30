#ifndef MSER_OPS_H
#define MSER_OPS_H
#include <QImage>
#include <QRgb>
#include <QColor>
#include <QVector>
#include <QPoint>
#include <QStack>
#include "component.h"

class mser_ops
{
public:

    QImage* inputGrayscaledImg;

    mser_ops();
    mser_ops(QImage* in);
    QImage threshold(int level, QImage* in);
    QVector<component> find_connected_components (QImage *in, int level);
    void trace_region_by_flooding(QImage* monoIn, int** visitedMap, int maxX, int maxY, QStack<QPoint>* seedPixels, int regionNo); // this method mutates visitedMap and seedPixels!
};

#endif // MSER_OPS_H
