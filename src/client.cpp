#include "client.h"

#include <QDebug>
#include <QHostAddress>
#include <QTimer>
#include <QTime>
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
Client::Client(QObject *parent)
    : Xml(parent)
{
    //Socket TCP: envoie des infos de jeu
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), this, SLOT(connexion())); //Client connecté au serveur
    connect(socket, SIGNAL(disconnected()), this, SLOT(deconnexion())); //Client déconnecté du serveur
    connect(socket, SIGNAL(readyRead()), this, SLOT(recevoir())); //Socket prêt à être lu
    //connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT());
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(erreurSocket(QAbstractSocket::SocketError))); //Erreur

    init_string = QString("Création du client inactif");

    portBroadcast = 34341;

    //Socket UDP: recherche des serveurs de jeu
    broadcastServeurs = new QUdpSocket(this);
//!
//    portLocal = 2000 + qrand()%5000;
//    while(broadcastServeurs->bind(portLocal) == false)
//        portLocal = 2000 + qrand()%5000;
    broadcastServeurs->bind(portBroadcast);

    connect(broadcastServeurs, SIGNAL(readyRead()), this, SLOT(receptionServeurOk()));



    QTimer::singleShot(0, this, SLOT(init())); //Signals qui ne sont pas émit dans le constructeur
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Client::init()
{
    emit log(init_string);
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Client::seConnecter(QString ip, int port)
{
    emit log(QString("Tentative de connexion à %1:%2").arg(ip).arg(port));

    socket->abort();
    socket->connectToHost(ip, port);
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Client::recherche()
{
    broadcastServeurs->writeDatagram("Je recherche des serveurs de jeu", QHostAddress::Broadcast, portBroadcast);

    emit log(QString("Recherche de serveurs"));
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Client::receptionServeurOk()
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
void Client::connexion()
{
    emit log(QString("Connecté au serveur"));
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Client::deconnexion()
{
    emit log(QString("Déconnecté du serveur"));
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Client::envoyer(QString texte)
{
    socket->write(texte.toUtf8());
    //emit log(QString("Envoi d'un packet"));
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Client::recevoir()
{
    QString texte = socket->readAll();
    lire(texte);

    emit recevoirOk(texte);
    //emit log(QString("Réception d'un packet"));
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Client::erreurSocket(QAbstractSocket::SocketError erreur)
{
    switch(erreur)
    {
        case QAbstractSocket::HostNotFoundError:
            emit erreurConnexion(QString("Le serveur est introuvable"));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            emit erreurConnexion(QString("Le serveur a refusé la connexion"));
            break;
        case QAbstractSocket::RemoteHostClosedError:
            emit erreurConnexion(QString("Le serveur a coupé la connexion"));
            break;
        default:
            emit erreurConnexion(QString("Erreur de connexion: " + socket->errorString()));
    }
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
