#include "rechercheserveurs.h"

#include <QTimer>
#include <QStringList>

#include <QDebug>
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
RechercheServeurs::RechercheServeurs(QObject *parent) :
    Xml(parent)
{
    portBroadcast = 34341;

    //Recherche des serveurs de jeu
    broadcastServeurs = new QUdpSocket(this);

    portLocal = 2000 + qrand()%5000;
    while(broadcastServeurs->bind(portLocal) == false)
        portLocal = 2000 + qrand()%5000;

    connect(broadcastServeurs, SIGNAL(readyRead()), this, SLOT(receptionServeurOk()));

    QTimer::singleShot(0, this, SLOT(init()));
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void RechercheServeurs::init()
{
    emit log(QString("Port du client-broadcast: %1").arg(portLocal));
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void RechercheServeurs::recherche()
{
    broadcastServeurs->writeDatagram("Je recherche des serveurs de jeu", QHostAddress::Broadcast, portBroadcast);

    emit log("Recherche de serveurs");
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void RechercheServeurs::receptionServeurOk()
{

    while(broadcastServeurs->hasPendingDatagrams())
    {
        QByteArray datagram;
        QString texte;
        QHostAddress sender;
        quint16 senderPort;

        datagram.resize(broadcastServeurs->pendingDatagramSize());
        broadcastServeurs->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        texte = datagram.data();

        emit log(QString("Reponse de %1:%2")
                                  .arg(sender.toString())
                                  .arg(senderPort));

        lire(texte);
    }
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
