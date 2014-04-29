#ifndef SERVEUR_H
#define SERVEUR_H

#include "xml.h"
#include "datas.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>

class Serveur : public QTcpServer
{
        Q_OBJECT
    public :
        Serveur(QObject *parent = 0, Datas *datas = 0);
        bool seConnecter(int port = 4000);
        QString nom()
        {
            return mNom;
        }
        void setNom(QString nom)
        {
            mNom = nom;
        }
        void envoyer(QString texte);
        void envoyer(int id, QString texte);

    private slots :
        void init();
        void connexion();
        void deconnexion();
        void recevoir();
        void debug(QString texte);
        void receptionBroadcastOk();

    signals :
        void log(QString);

    private :
        QString mNom;
        Datas *datas;
        Xml *xml;
        QString init_string;
        QUdpSocket *receptionBroadcast; //2. serveur receptionne le broadcast du client
        QUdpSocket *envoiClient;
        int idIndex;
        quint16 portBroadcast;
        QStringList ipServeur;
};

#endif // SERVEUR_H
