#include "karbowysegmtekst17_18.h"
#include "apoimgloaders.h"
#include "mser_ops.h"

KarbowySegmTekst17_18::KarbowySegmTekst17_18(QWidget *parent) : QWidget(parent) { // Constructor for the main window. See header file
                                                                                  // for fields and methods declarations.


    // MAIN WINDOW BUTTONS
    btn_load = new QPushButton("Load file");
    btn_grayscale = new QPushButton("Grayscale");
    btn_test = new QPushButton("Test");
    btn_quit = new QPushButton("Quit");
    QObject::connect(btn_quit, SIGNAL(clicked()), this, SLOT(bye()));
    QObject::connect(btn_grayscale, SIGNAL(clicked()), this, SLOT(grayscaleImage()));
    QObject::connect(btn_test, SIGNAL(clicked()), this, SLOT(testFun()));
    QObject::connect(btn_load, SIGNAL(clicked()), this, SLOT(loadImage()));

    buttons_bar = new QHBoxLayout;
    buttons_bar->addWidget(btn_load);
    buttons_bar->addWidget(btn_grayscale);
    buttons_bar->addWidget(btn_test);
    buttons_bar->addWidget(btn_quit);
    buttons_group = new QGroupBox();
    buttons_group->setLayout(buttons_bar);
    buttons_group->setMaximumHeight(100);

    // MAIN WINDOW LAYOUT
    pionowy = new QVBoxLayout;
    pionowy->addWidget(buttons_group);


    etykieta = new QLabel("Hello, world!");
    imageScrollArea = new QScrollArea();
    imageScrollArea->setWidget(etykieta);
    pionowy->addWidget(imageScrollArea);
    biezacyObraz=NULL;


    // GLOWNE OKNO
    this->setLayout(pionowy);

}

void KarbowySegmTekst17_18::bye() {
    exit(0);
}

void KarbowySegmTekst17_18::loadImage() {
    QImage* tymcz = ApoImgLoaders::loadImageFile(etykieta);
    if (tymcz!=NULL) {
        QString str;
        str.sprintf("Loaded picture. W=%d H=%d", tymcz->width(), tymcz->height());
        qDebug(str.toLatin1());
        biezacyObraz=tymcz;
    }
    else biezacyObraz=NULL;
}

void KarbowySegmTekst17_18::grayscaleImage() {
    if (biezacyObraz==NULL) {
        qDebug("Please load a picture");
        return;
    }
    QImage* grayscaled=ApoImgLoaders::toGrayscale(biezacyObraz);
    etykieta->setPixmap(QPixmap::fromImage(*grayscaled));
    etykieta->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    etykieta->setScaledContents(false);
    etykieta->adjustSize();

}

void KarbowySegmTekst17_18::testFun() {
    mser_ops* letsroll = new mser_ops(biezacyObraz);
    ApoImgLoaders::printAsciiRepresentation(biezacyObraz);
    QVector<component> leaves;

//    int** componentTrackerMap = new int*[biezacyObraz->width()];
//    for (int x=0; x<biezacyObraz->width(); x++) componentTrackerMap[x]=new int[biezacyObraz->height()];
//    for (int y=0; y<biezacyObraz->height(); y++) {
//        for (int x=0; x<biezacyObraz->width(); x++) {
//            componentTrackerMap[x][y]=0;
//        }
//    }


    QImage incremental = QImage(biezacyObraz->size(), QImage::Format_Mono);
    QPainter painter(&incremental);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(incremental.rect(), Qt::black);
    painter.setCompositionMode(QPainter::CompositionMode_Plus);

    QElapsedTimer timer;
    timer.start();
    for (int a=0; a<256; a++) {
        QImage thresholded = letsroll->threshold(a, NULL);
        qDebug() << "Processing level " << a;
        // 1. Get all contiguous regions of white pixels
        QVector<component> found_components = letsroll->find_connected_components(&thresholded, a);
        if (found_components.size()>0) {
            qDebug() << "At level " << a << " found " << found_components.size() << " connected component(s). Here they are:";
            foreach (component c, found_components) {
                c.printComponent(COORDINATES, thresholded.width(), thresholded.height());
                c.calculateBoundary(EIGHT); // 1a. Calculate each region's boundary
                c.printComponent(BOUNDARY, thresholded.width(), thresholded.height());
            }
        }

        // 2. For all boundary pixels in regions detected in point 1:
        //    Check if they have neighbours in existing regions (i.e those in incremental image - before drawing the currently discovered region):
        //      - If yes, glue them to these regions.
        //      - If not, add a new leaf to the component tree
        painter.drawImage(0, 0, thresholded);
        etykieta->setPixmap(QPixmap::fromImage(incremental));
     //   etykieta->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
     //   etykieta->setScaledContents(false);

 //       QString str;
   //     str.sprintf("Processing level %d", a);
     //   qDebug(str.toAscii());
      //  etykieta->setText(str);
       //    QTime dieTime= QTime::currentTime().addMSecs(25);
      //     while (QTime::currentTime() < dieTime)
      //          QCoreApplication::processEvents(QEventLoop::AllEvents, 20);


    }
    painter.end();
    delete letsroll;

    qDebug() << timer.elapsed();
    QPoint* p1 = new QPoint(1,1);
    QPoint* p2 = new QPoint(1,2);
    QPoint* p3 = new QPoint(1,1);
    QPoint* p4 = new QPoint(0,0);
    QVector<QPoint> vv;
    vv.push_back(*p1); vv.push_back(*p2); vv.push_back(*p3);
    qDebug() << "p1 eq p2?: " << (*p1==*p2) << "p1 eq p3?: " << (*p1==*p3) << "p2 eq p3?: " << (*p2==*p3);
    qDebug() << "Vector contains 1,1? " << vv.contains(*p4);
    // if (thresholded==NULL) return;


// ***********************************************************************
//    if (biezacyObraz==NULL) {
//        qDebug("Please load a picture");
//        return;
//    }
//    QVector<QRgb> tmp = ApoImgLoaders::getNeighboringPixels(FOUR, biezacyObraz, 10, 10, true);
//    ApoImgLoaders::printQRgbTable(tmp);
}


KarbowySegmTekst17_18::~KarbowySegmTekst17_18()
{
    
}
