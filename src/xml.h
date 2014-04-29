#ifndef XML_H
#define XML_H

#include <QObject>
#include <QXmlStreamWriter>
#include <QPointF>
#include <QHostAddress>
#include <QStringList>

class Joueur;
class Missile;
#include "horloge.h"

class Xml : public QObject
{
        Q_OBJECT
    public:
        explicit Xml(QObject *parent = 0, Horloge *horloge = 0);

    signals:
        //Signaux émis par le slot lire()
        void lireIdPerso(QStringList);
        void lireIdJoueur(QStringList);
        void lireIdRobot(QStringList);
        void lireInfoJoueur(QStringList);
        void lireInfoServeur(QStringList);
        void lireDeconnexion(QStringList);
        void lireAction(QStringList);
        void lireMessage(QStringList);
        void lireTir(QStringList);
        void lireCollision(QStringList);
        void lireDemandeSynchronisationHorloge(QStringList);
        void lireReponseSynchronisationHorloge(QStringList);

    public slots:
        //Slot qui émet différents signaux (emit lireNomOk();)
        void lire(QString source);

        //Slots qui écrivent différentes données
        QByteArray ecrireIdPerso(int id);
        QByteArray ecrireIdPerso(Joueur *joueur);

        QByteArray ecrireIdJoueur(int id);
        QByteArray ecrireIdJoueur(Joueur *joueur);

        QByteArray ecrireIdRobot(int id);
        QByteArray ecrireIdRobot(Joueur *joueur);

        QByteArray ecrireInfoJoueur(int id, QString pseudo, bool admin, bool hote, int vie, int score);
        QByteArray ecrireInfoJoueur(Joueur *joueur);

        QByteArray ecrireDeconnexion(int id);
        QByteArray ecrireDeconnexion(Joueur *joueur);

        QByteArray ecrireAction(int id, int action, QPointF pos, double angle, double vitesse);
        QByteArray ecrireAction(Joueur *joueur);

        QByteArray ecrireMessage(int id, QString message);
        QByteArray ecrireMessage(Joueur *joueur, QString message);

        QByteArray ecrireTir(int idJoueur, int idMissile, QPointF position, double angle, int type);
        QByteArray ecrireTir(Missile *missile);

        QByteArray ecrireCollision(int idTireur, int scoreTireur, int idExplose, int vieExplose, int idMissile);
        QByteArray ecrireCollision(Joueur *joueurTireur, Joueur *joueurExplose, Missile *missile);

        QByteArray ecrireInfoServeur(QString ip, int port, QString nom, int total);

        QByteArray ecrireDemandeSynchronisationHorloge(int id, int dernierPing);
        QByteArray ecrireDemandeSynchronisationHorloge(Joueur *joueur);

        QByteArray ecrireReponseSynchronisationHorloge();

    private slots:
        QString temps()
        {
            return QString::number(mHorloge->heureServeur());
        }


    private:
        QString texte;
        QXmlStreamWriter *xml;
        Horloge *mHorloge;
};

#endif // XML_H
