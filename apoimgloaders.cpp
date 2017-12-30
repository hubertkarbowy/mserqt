#include "apoimgloaders.h"

ApoImgLoaders::ApoImgLoaders() { }

void ApoImgLoaders::dummy() {
    qDebug("xyz");
}

void ApoImgLoaders::dummy2() {
    qDebug("dummy2");
}

void ApoImgLoaders::bye() {
    exit(0);
}

QImage* ApoImgLoaders::loadImageFile(QLabel *etykieta) {
    QString sciezka = QFileDialog::getOpenFileName(NULL, "Select image file to load", "/home/hubert/Bilder");
    QImage* poKonwersjiKolorow;
    poKonwersjiKolorow = new QImage(sciezka);
    *poKonwersjiKolorow = poKonwersjiKolorow->convertToFormat(QImage::Format_RGB32);
    if (poKonwersjiKolorow->isNull()) {
        qDebug("Unsupported image file format");
        etykieta->setText("Unsupported image file format");
        etykieta->adjustSize();
        return NULL;
    }
    else qDebug("Loaded image");
    etykieta->setPixmap(QPixmap::fromImage(*poKonwersjiKolorow));
    etykieta->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    etykieta->setScaledContents(false);
    etykieta->adjustSize();
    return poKonwersjiKolorow;
}

QImage* ApoImgLoaders::toGrayscale(QImage *obraz) {

    if (obraz==NULL) {
        qDebug("Please load a picture before grayscaling");
        return NULL;
    }

    QString str;
    str.sprintf("W=%d H=%d", obraz->width(), obraz->height());
    qDebug(str.toLatin1());

    for (int ii = 0; ii < obraz->height()  ; ii++) {
            uchar* scan = obraz->scanLine(ii);
            int depth =4;
            for (int jj = 0; jj < obraz->width(); jj++) {
                QRgb* rgbpixel = reinterpret_cast<QRgb*>(scan + jj*depth);
                int gray = qGray(*rgbpixel);
//                if (ii==263 && jj==176) {
//                    QString pxcol;
//                    pxcol.sprintf("10x10 COL=%d", gray);
//                    qDebug(pxcol.toLatin1());
//                }
                *rgbpixel = QColor(gray, gray, gray).rgba();
            }
        }
  //  poKonwersjiKolorow = new QImage(sciezka);
   // *obraz = obraz->convertToFormat(QImage::Format_Indexed8);
    qDebug("Done converting RGB -> Grayscale");
    return obraz;
}

QVector<QRgb> ApoImgLoaders::getNeighboringPixels (neighborhoodType neighborhood, QImage* referenceImage, int x, int y, bool includeCenterPixel=true) {
    QVector<QRgb> neighboringPixelValues;
    if (neighborhood==FOUR) {
        neighboringPixelValues.push_back(referenceImage->pixel(x,y-1));
        neighboringPixelValues.push_back(referenceImage->pixel(x-1,y));
        if (includeCenterPixel) neighboringPixelValues.push_back(referenceImage->pixel(x,y));
        neighboringPixelValues.push_back(referenceImage->pixel(x+1,y));
        neighboringPixelValues.push_back(referenceImage->pixel(x,y+1));
    }
    else if (neighborhood==EIGHT) {
        neighboringPixelValues.push_back(referenceImage->pixel(x-1,y-1));
        neighboringPixelValues.push_back(referenceImage->pixel(x,y-1));
        neighboringPixelValues.push_back(referenceImage->pixel(x+1,y-1));
        neighboringPixelValues.push_back(referenceImage->pixel(x-1,y));
        if (includeCenterPixel) neighboringPixelValues.push_back(referenceImage->pixel(x,y));
        neighboringPixelValues.push_back(referenceImage->pixel(x+1,y));
        neighboringPixelValues.push_back(referenceImage->pixel(x-1,y+1));
        neighboringPixelValues.push_back(referenceImage->pixel(x,y+1));
        neighboringPixelValues.push_back(referenceImage->pixel(x+1,y+1));
    }
    return neighboringPixelValues;
}

QVector<QPoint> ApoImgLoaders::createNeighborhood (int x, int y, int maxX, int maxY, neighborhoodType type=FOUR) {
    QVector<QPoint> neighboringPoints;
    if (x<1 || y < 1 || x>maxX-2 || y > maxY-2) return neighboringPoints; // cheating around edge pixels
    if (type==FOUR) {
        neighboringPoints.push_back(QPoint(x, y-1));
        neighboringPoints.push_back(QPoint(x-1, y));
        neighboringPoints.push_back(QPoint(x+1, y));
        neighboringPoints.push_back(QPoint(x, y+1));
    }
    else if (type==EIGHT) {
        neighboringPoints.push_back(QPoint(x-1, y-1));
        neighboringPoints.push_back(QPoint(x, y-1));
        neighboringPoints.push_back(QPoint(x+1, y-1));
        neighboringPoints.push_back(QPoint(x-1, y));
        neighboringPoints.push_back(QPoint(x+1, y));
        neighboringPoints.push_back(QPoint(x-1, y+1));
        neighboringPoints.push_back(QPoint(x, y+1));
        neighboringPoints.push_back(QPoint(x+1, y+1));
    }
    return neighboringPoints;
}

void ApoImgLoaders::printQRgbTable(QVector<QRgb> vec) {
    foreach (QRgb px, vec) { // poorly implemented with Qt - makes a copy of the containers. Consider using boost.
       QString thisPxValue;
       thisPxValue.sprintf("R=%d, G=%d, B=%d", qRed(px), qGreen(px), qBlue(px));
       qDebug(thisPxValue.toLatin1());
    }
}

void ApoImgLoaders::printAsciiRepresentation(QImage* in) {
    qDebug() << "Here is a text representation (1 = pixel value 100, 2 = pixel value 200, * = pixel value 255, 0 = all other pixel values";
    for (int y=0; y<in->height(); y++) {
        QString thisLine;
        for (int x=0; x<in->width(); x++) {
            QRgb thisPixel = in->pixel(x,y);
            if (qRed(thisPixel)==100) thisLine.append("1");
            else if (qRed(thisPixel)==200) thisLine.append("2");
            else if (qRed(thisPixel)==255) thisLine.append("*");
            else thisLine.append("0");
        }
        qDebug(thisLine.toAscii());
    }
}
