#include <QtGui/QApplication>
#include "karbowysegmtekst17_18.h"
#include "apoimgloaders.h"

int main(int argc, char *argv[])
{
    ApoImgLoaders *ladow;
    QApplication a(argc, argv);

    KarbowySegmTekst17_18 w; // obiekt typu QWidget
    qDebug("aaaa");

    w.show();
    
    return a.exec();
}
