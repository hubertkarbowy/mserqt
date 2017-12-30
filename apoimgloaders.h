#ifndef APOIMGLOADERS_H
#define APOIMGLOADERS_H
#include <QtGui/QWidget>
#include <QLabel>
#include <QFileDialog>
#include <QImage>
#include <QImageReader>
#include <QScrollArea>
#include <QVector>
#include <QRgb>
#include <QDebug>
#include "constdefinitions.h"

class ApoImgLoaders : public QObject
{
    Q_OBJECT

public:
    ApoImgLoaders();
    void dummy2();
    static QImage* loadImageFile(QLabel *etykieta);
    static QImage* toGrayscale(QImage *obraz);
    static QVector<QRgb> getNeighboringPixels(neighborhoodType neighborhood, QImage* referenceImage, int x, int y, bool includeCenterPixel);
    static QVector<QPoint> createNeighborhood (int x, int y, int maxX, int maxY, neighborhoodType type);
    static void printQRgbTable(QVector<QRgb> vec);
    static void printAsciiRepresentation(QImage* in);

public slots:
    void dummy();
    void bye();
};

#endif // APOIMGLOADERS_H
