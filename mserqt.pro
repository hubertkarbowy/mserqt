#-------------------------------------------------
#
# Project created by QtCreator 2017-12-27T23:27:03
#
#-------------------------------------------------

QT       += core gui


TARGET = mserqt
TEMPLATE = app


SOURCES += main.cpp\
        karbowysegmtekst17_18.cpp \
    apoimgloaders.cpp \
    component.cpp \
    mser_ops.cpp

HEADERS  += karbowysegmtekst17_18.h \
    apoimgloaders.h \
    constdefinitions.h \
    component.h \
    mser_ops.h

FORMS += \
    dialog.ui

OTHER_FILES += \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog


maemo5 {
    target.path = /opt/mserqt/bin
    INSTALLS += target
}
