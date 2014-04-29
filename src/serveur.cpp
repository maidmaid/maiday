#include "serveur.h"

#include "joueur.h"

#include <QDebug>
#include <QTimer>
#include <QNetworkInterface>
#include <QTime>

//-----------------------------------------------------------------------------------------------------------------------------
//Constructeur
Serveur::Serveur(QObject *parent, Datas *datas) : QTcpServer(parent)
{
    this->datas = datas;

    portBroadcast = 34341;

    setNom("MAIDay's LAN");

    //Formatage des données en XML
    xml = new Xml(this, datas->moi->horloge());

    //Nouvelle connection d'un client au serveur de jeu
    connect(this, SIGNAL(newConnection()), this, SLOT(connexion()));

    //Reception des requêtes broadcast des clients
    receptionBroadcast = new QUdpSocket(this);
    connect(receptionBroadcast, SIGNAL(readyRead()), this, SLOT(receptionBroadcastOk()));

    //Envoi les infos serveurs au client qui recherches des serveurs
    envoiClient = new QUdpSocket(this);

    //Signals qui ne sont pas émit dans le constructeur
    init_string = QString("Création d'un serveur inactif");
    QTimer::singleShot(0, this, SLOT(init()));
}
//-----------------------------------------------------------------------------------------------------------------------------
//Initialisation du serveur
void Serveur::init()
{
    emit log(init_string);
}
//-----------------------------------------------------------------------------------------------------------------------------
//Rend actif le serveur
bool Serveur::seConnecter(int port)
{
    bool ok;

    close();
    ipServeur.clear();

    //Ecoute sur toutes les ip du PC
    if(listen(QHostAddress::Any, port))
    {
        emit log(QString("Démarrage du serveur:"));

        //Recherche des adresses IP du serveur en parcourant toutes les ip du PC
        QList<QHostAddress> ips = QNetworkInterface::allAddresses();
        for (int i = 0; i < ips.size(); ++i)
        {
            //Si l'ip est autre que 127.0.0.1 et est v4
            if (ips.at(i) != QHostAddress::LocalHost && ips.at(i).toIPv4Address())
                ipServeur << ips.at(i).toString();
        }
        emit log(QString("Port : %1 IP : %2")
                 .arg(port)
                 .arg(ipServeur.join("-")));

        receptionBroadcast->bind(portBroadcast);

        //Réinitialise l'index
        idIndex = 0;

        //! Démmare l'horloge du serveur
        datas->moi->setId(1);
        datas->moi->horloge()->synchroniser(0, 0);

        ok = true;
    }
    else
    {
        ok = false;

        emit log(QString("Erreur serveur: %1")
                 .arg(errorString()));
    }

    return ok;
}
//-----------------------------------------------------------------------------------------------------------------------------
//Debug
void Serveur::debug(QString texte)
{
    qDebug() << texte;
}
//-----------------------------------------------------------------------------------------------------------------------------
//Slot réceptionnant une connexion d'un nouveau client
void Serveur::connexion()
{
    QString donnees; //Données envoyées au nouveau client

    //Donne un id au nouveau client
    idIndex++;

    //Sauvegarde les données du nouveau client
    Joueur *joueur = new Joueur(this);
    joueur->setId(idIndex);
    joueur->setSocket(nextPendingConnection());
    joueur->socket()->setObjectName(QString("client%1").arg(idIndex));
    if(idIndex == 1) //admin si c'est le premier
    {
        joueur->setAdmin(true);
        joueur->setHote(true);
    }
    datas->ajouterJoueur(joueur);

    //Reception sur le serveur des packets du nouveau client
    connect(joueur->socket(), SIGNAL(readyRead()), this, SLOT(recevoir()));
    //Déconnexion du nouveau client
    connect(joueur->socket(), SIGNAL(disconnected()), this, SLOT(deconnexion()));

    //Envoi au nouveau joueur son id
    donnees.clear();
    donnees.append(xml->ecrireIdPerso(joueur));

    //Envoi au nouveau joueur la liste des joueurs
    foreach (Joueur *i, datas->joueurs)
    {
        if(i->id() != idIndex)
        {
            donnees.append(xml->ecrireIdJoueur(i));
            donnees.append(xml->ecrireInfoJoueur(i));
            donnees.append(xml->ecrireAction(i));
        }
    }
    envoyer(idIndex, donnees);

    //Envoi à tous les clients l'id du nouveau joueur
    envoyer(xml->ecrireIdJoueur(joueur));

    emit log(QString("Nouvelle connexion (%1:%2)")
             .arg(joueur->socket()->peerAddress().toString())
             .arg(joueur->socket()->peerPort()));
}
//-----------------------------------------------------------------------------------------------------------------------------
//Slot réceptionnant la déconnexion d'un client
void Serveur::deconnexion()
{
    int id;

    //Récupère l'id
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    id = client->objectName().mid(6).toInt();

    //Envoi à tous les clients l'id du joueur déconnecté
    envoyer(xml->ecrireDeconnexion(id));

    emit log(QString("Déconnexion d'un client (%1:%2)")
             .arg(client->peerAddress().toString())
             .arg(client->peerPort()));
}
//-----------------------------------------------------------------------------------------------------------------------------
//Slot réceptionnant l'arrivé d'un nouveau message d'un client
void Serveur::recevoir()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender()); // On détermine quel client envoie le message (recherche du QTcpSocket du client)
    QString texte = client->readAll();

    //    emit log(QString("Récéption d'un packet (%1:%2)")
    //                .arg(client->peerAddress().toString())
    //                .arg(client->peerPort()));

    envoyer(texte);
}
//-----------------------------------------------------------------------------------------------------------------------------
//Envoie à tous les clients un message
void Serveur::envoyer(QString texte)
{
    //Envoie à tout les clients
    foreach (Joueur *i, datas->joueurs)
    {
        i->socket()->write(texte.toUtf8());
    }

    //emit log(QString("Renvoi du packet à tout le monde"));
}
//-----------------------------------------------------------------------------------------------------------------------------
//Envoie à un client spécifique un message
void Serveur::envoyer(int id, QString texte)
{
    //chercherClient(id)->write(texte.toUtf8());
    datas->chercherJoueur(id)->socket()->write(texte.toUtf8());

    //emit log(QString("Renvoi du packet à %1").arg(id));
}
//-----------------------------------------------------------------------------------------------------------------------------
//Receptionne une demande de recherche de serveurs
void Serveur::receptionBroadcastOk()
{
    QHostAddress ip;
    int mask;
    QString ipIsInSubnet;
    ipIsInSubnet.clear();

    while(receptionBroadcast->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress sender;
        quint16 senderPort;

        datagram.resize(receptionBroadcast->pendingDatagramSize());
        receptionBroadcast->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        emit log(QString("Reception 'recherche de serveur' (%1:%2)")
                 .arg(sender.toString())
                 .arg(senderPort));

        //Recherche l'adresse ip faisant parti du même réseau
        for(mask = 32; mask > 0; mask--)
        {
            foreach (QString ip, ipServeur)
            {
                if(sender.isInSubnet(QHostAddress(ip), mask))
                {
                    ipIsInSubnet = ip;
                    mask = 0;
                }
            }
        }

        //Envoi au client des infos serveurs
        envoiClient->writeDatagram(xml->ecrireInfoServeur(ipIsInSubnet, serverPort(), nom(), datas->joueurs.size()),
                                   sender,
                                   senderPort);

        //        emit log(QString("Envoi des infos serveur (%1:%2)")
        //                    .arg(sender.toString())
        //                    .arg(senderPort));

    }

}
