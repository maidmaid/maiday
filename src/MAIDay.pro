#-------------------------------------------------
#
# Project created by QtCreator 2011-04-08T20:42:00
#
#-------------------------------------------------

QT       += core gui \
            network \
            xml

TARGET = MAIDay
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    xml.cpp \
    serveur.cpp \
    client.cpp \
    avion.cpp \
    affichage.cpp \
    joueur.cpp \
    datas.cpp \
    console.cpp \
    missile.cpp \
    collision.cpp \
    explosion.cpp \
    objetvolant.cpp \
    horloge.cpp \
    chargement.cpp

HEADERS  += mainwindow.h \
    xml.h \
    serveur.h \
    client.h \
    avion.h \
    affichage.h \
    joueur.h \
    datas.h \
    console.h \
    missile.h \
    collision.h \
    explosion.h \
    objetvolant.h \
    horloge.h \
    chargement.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    doxygen.txt











