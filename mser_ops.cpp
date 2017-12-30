#include "mser_ops.h"
#include "apoimgloaders.h"
#include "constdefinitions.h"

mser_ops::mser_ops() {
}

mser_ops::mser_ops(QImage* in) {
    inputGrayscaledImg=in;
}

QImage mser_ops::threshold(int level, QImage* in) {
    QImage* which = in==NULL ? inputGrayscaledImg : in;
    if (which==NULL) {
        qDebug("No input image for thresholding op");
       // return NULL;
    }

    QImage mask1 = which->createMaskFromColor(qRgb(level,level,level), Qt::MaskInColor); // TODO: ditch qGray to improve efficiency
     return mask1;

     // ******************************************************************
    QImage* faster = new QImage(which->width(), which->height(), QImage::Format_RGB32);

    QRgb* oriscan = reinterpret_cast<QRgb*>(which->bits());
    uchar* fscan = faster->bits();
    quint64 pixelCount = which->width() * which->height();
    uchar eight=0;




//    for (quint64 p = 0; p < pixelCount; p++) {
//        if ((p%8)==0) fscan[(p/8)]=eight;
//        QRgb rgbpixel = oriscan[p];
//        if (qRed(rgbpixel)==level) eight=((eight<<1)|1);
//        else eight=(eight<<1);
//    }
  //  uchar* newdata = faster->numBytes();

   // QString zz;
    //zz.sprintf("Original bytes = %d, mono = %d, pc=%d", which->numBytes(), faster->numBytes(), pixelCount);
   // qDebug(zz.toAscii());
    return *faster;
//    for (int ii = 0; ii < which->height()  ; ii++) {
//            uchar* scan = which->scanLine(ii);
//            uchar* outscan = faster->scanLine(ii);
//            for (int jj = 0; jj < which->width(); jj++) {
//                QRgb* rgbpixel = reinterpret_cast<QRgb*>(scan + jj*depth);
//              //  *outscan = *rgbpixel;
//                *(outscan+jj*depth) = *rgbpixel;
//                //int gray = qGray(*rgbpixel);
//                //faster->fromData()
//            }

//    }

    return *which;
}

void mser_ops::trace_region_by_flooding(QImage* monoIn, int** visitedMap, int maxX, int maxY, QStack<QPoint>* seedPixels, int regionNo) {
    if (!seedPixels->empty()) {
        QPoint currentPoint = seedPixels->pop(); // STEP 1. Pop the current pixel from stack and save the current region's number in the map.
        int rx = currentPoint.rx();
        int ry = currentPoint.ry();
        visitedMap[rx][ry]=regionNo;
        QVector<QPoint> neighboringPixels = ApoImgLoaders::createNeighborhood(rx,ry,maxX,maxY,EIGHT); // STEP 2. Get all neighboring pixels
        foreach (QPoint p, neighboringPixels) {
            if (qRed(monoIn->pixel(p))==255 && visitedMap[p.rx()][p.ry()]==0) { // STEP 2a. If any neighboring pixel is unexplored, push it to stack
                seedPixels->push(p);
            }
        }
        mser_ops::trace_region_by_flooding(monoIn, visitedMap, maxX, maxY, seedPixels, regionNo); // STEP 3. Call recursively. Recursion stops if the seed pixels stack is empty.
    }
}

QVector<component> mser_ops::find_connected_components (QImage *monoIn, int level) {

    int** visitedMap = new int*[monoIn->width()];
    for (int x=0; x<monoIn->width(); x++) visitedMap[x]=new int[monoIn->height()];
    for (int y=0; y<monoIn->height(); y++) {
        for (int x=0; x<monoIn->width(); x++) {
            visitedMap[x][y]=0;
        }
    }
    bool noChanges=false;
    neighborhoodType ntype=EIGHT;
    int regionNo=1;
    QStack<QPoint> seedPixels;

    // if (level==100 || level==200) ApoImgLoaders::printAsciiRepresentation(monoIn); // HKDEBUG

    while (!noChanges) {
        noChanges=true;
        for (int y=1; y<monoIn->height()-1; y++) {
            for (int x=1; x<monoIn->height()-1; x++) {
                QRgb thisPixel = monoIn->pixel(x,y);
                if (visitedMap[x][y]>0) continue;
                else if (qRed(thisPixel)==255) {
                    noChanges=false;
                    seedPixels.push(QPoint(x,y));
                    trace_region_by_flooding(monoIn, visitedMap, monoIn->width(), monoIn->height(), &seedPixels, regionNo); // guaranteed to find all pixels contiguous to (x,y) and mark them as regionNo in visitedMap
                    regionNo++; // it is now safe to increment the region counter
                }
            }
        }
    }



    //HKDEBUG start
//        QString metrics;
//        metrics.sprintf("Number of detected regions = %d", regionNo-1);
//        qDebug(metrics.toLatin1());
    //HKDEBUG end

    QVector<component>* retVal = new QVector<component>(regionNo-1);
    if (regionNo-1>0) {
           for (int y=0; y<monoIn->height(); y++) {
                for (int x=0; x<monoIn->width(); x++) {
                    int whichRegionIsIt=visitedMap[x][y];
                    if (whichRegionIsIt>0) {
                        component tmp = retVal->at(whichRegionIsIt-1);
                        tmp.addPoint(x,y);
                    }
                }
            }
    }

    for (int y=0; y<monoIn->height(); y++) { // deallocate memory
            delete[] visitedMap[y];
    }
    delete[] visitedMap;
    // return (QVector<component>)NULL;
    return *retVal;
}
