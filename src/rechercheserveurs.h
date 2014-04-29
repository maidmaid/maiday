#ifndef RECHERCHESERVEURS_H
#define RECHERCHESERVEURS_H

#include "xml.h"
#include <QObject>
#include <QUdpSocket>
#include <QStringList>


class RechercheServeurs : public Xml
{
    Q_OBJECT
public:
    explicit RechercheServeurs(QObject *parent = 0);
    void recherche();

signals:
    void log(QString);

private slots:
    void init();
    void receptionServeurOk();

private:
    quint16 portBroadcast;
    quint16 portLocal;

    QUdpSocket *broadcastServeurs; //1. on broadcast tous les serveurs
};

#endif // RECHERCHESERVEURS_H
