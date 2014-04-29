#ifndef CLIENT_H
#define CLIENT_H

#include "xml.h"

#include <QTcpSocket>
#include <QUdpSocket>

class Client : public Xml
{
        Q_OBJECT
    public :
        Client(QObject *parent = 0);
        void seConnecter(QString ip = "127.0.0.1", int port = 4000);

    public slots :
        void envoyer(QString texte);
        void recherche();

    private slots :
        void init();
        void connexion();
        void deconnexion();
        void recevoir();
        void erreurSocket(QAbstractSocket::SocketError erreur);
        void receptionServeurOk();


    signals :
        void erreurConnexion(QString);
        void log(QString);

        void connexionOk(QString);
        void deconnexionOk(QString);
        void recevoirOk(QString);

    private :
        QString init_string;
        QTcpSocket *socket;

        quint16 portBroadcast;
        quint16 portLocal;

        QUdpSocket *broadcastServeurs; //1. on broadcast tous les serveurs

};


#endif // CLIENT_H
