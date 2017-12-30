#ifndef KARBOWYSEGMTEKST17_18_H
#define KARBOWYSEGMTEKST17_18_H

#include <QtGui/QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QImage>
#include <QPainter>
#include <QScrollArea>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QDebug>
#include <QElapsedTimer>


class KarbowySegmTekst17_18 : public QWidget
{
    Q_OBJECT
    
public:
    KarbowySegmTekst17_18(QWidget *parent = 0);

    QGroupBox *buttons_group;
    QHBoxLayout *buttons_bar;
    QPushButton *btn_load;
    QPushButton *btn_grayscale;
    QPushButton *btn_test;
    QPushButton *btn_quit;
    QVBoxLayout *pionowy;
    QLabel *etykieta;
    QImage *biezacyObraz;
    QScrollArea *imageScrollArea;

    ~KarbowySegmTekst17_18();

public slots:
    void bye();
    void loadImage();
    void grayscaleImage();
    void testFun();
};

#endif // KARBOWYSEGMTEKST17_18_H
