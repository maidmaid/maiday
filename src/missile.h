#ifndef MISSILE_H
#define MISSILE_H

//Class héritage
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QThread>

#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QTime>
#include <math.h>

#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>

#include <QTimer>

#include "xml.h"
#include "explosion.h"

class Datas;

class Missile : public QObject, public QGraphicsPixmapItem
{
        Q_OBJECT
        //-------------------------------------------------------------
        Q_PROPERTY(QPointF position READ position WRITE setPosition)
        Q_PROPERTY(double angle READ angle WRITE setAngle)
        Q_PROPERTY(double vitesse READ vitesse WRITE setVitesse)
        //-------------------------------------------------------------
        Q_PROPERTY(double acceleration READ acceleration WRITE setAcceleration)
        Q_PROPERTY(double accelerationMax READ accelerationMax WRITE setAccelerationMax)
        Q_PROPERTY(double angleIncrement READ angleIncrement WRITE setAngleIncrement)
        Q_PROPERTY(double angleMax READ angleMax WRITE setAngleMax)
        Q_PROPERTY(double vitesseMin READ vitesseMin WRITE setVitesseMin)
        Q_PROPERTY(double vitesseMax READ vitesseMax WRITE setVitesseMax)
        //-------------------------------------------------------------
        Q_PROPERTY(int looping READ looping WRITE setLooping)
        //-------------------------------------------------------------

    public:
        enum typeMissile
        {
            typeNormal,
            typeZigzag,
            typeTournantDroite,
            typeTournantGauche
        };
        enum actionMissile
        {
            accelere,
            freine,
            plane,
            tourneADroite,
            tourneAGauche,
            tournePlus
        };
        enum explosionMissile
        {
            explosionTimeout,
            explosionCollision,
            explosionKill
        };

        //Constructeur
        explicit Missile(QObject *parent = 0,
                         QGraphicsScene *scene = 0,
                         Datas *datas = 0,
                         int idJoueurTireur = 0,
                         int id = 0,
                         QPointF position = QPointF(0,0),
                         double angle = 0,
                         typeMissile type = typeNormal);
        ~Missile();

        //Métodes liées aux propriétés de la class (méthodes de lecture)
        int id;
        int idJoueurTireur;
        int idJoueurExplose;
        QPointF newPosition;
        double newAngle;
        double newVitesse;
        bool modeManuel;

        QPointF position()
        {
            return mPosition;
        }
        double angle()
        {
            return mAngle;
        }
        double vitesse()
        {
            return mVitesse;
        }
        double acceleration()
        {
            return mAcceleration;
        }
        double accelerationMax()
        {
            return mAccelerationMax;
        }
        double angleIncrement()
        {
            return mAngleIncrement;
        }
        double angleMax()
        {
            return mAngleMax;
        }
        double vitesseMin()
        {
            return mVitesseMin;
        }
        double vitesseMax()
        {
            return mVitesseMax;
        }
        int looping()
        {
            return mLooping;
        }
        int typeDeMissile()
        {
            return mType;
        }
        void setTypeExplosion(explosionMissile typeExplosion)
        {
            this->typeExplosion = typeExplosion;
        }
        void setCollision(bool collision)
        {
            mIsCollision = collision;
        }

        bool isCollision()
        {
            return mIsCollision;
        }

        //Méthodes appellées par l'utilisateur pour diriger son avion
        void accelerer()
        {
            setAcceleration(accelerationMax());
            action = accelere;
        }
        void freiner()
        {
            setAcceleration(-accelerationMax());
            action = freine;
        }
        void planer()
        {
            setAcceleration(0);
            action = plane;
        }
        void droite()
        {
            setAngleIncrement(angleMax());
            action = tourneADroite;
        }
        void gauche()
        {
            setAngleIncrement(-angleMax());
            action = tourneAGauche;
        }
        void toutDroit()
        {
            setAngleIncrement(0);
            action = tournePlus;
        }



    signals:
        //Permet d'envoyer au serveur via la class Client les infos sur l'avion
        void explose();
        void infoSortante(QString);

    public slots:
        //Permet de recevoir des informations du serveur
        //void infoEntrante(QString texte);
        void zigzag();
        //Explosion
        void exploser();

    private slots:
        //Métodes liées aux propriétés de la class (méthodes d'écritures)
        void setPosition(QPointF position)
        {
            mPosition = position;
            setPos(mPosition);
        }
        void setAngle(double angle)
        {
            if(angle >= 360)
                mAngle = angle - 360;
            else if(angle < 0)
                mAngle = angle + 360;
            else
                mAngle = angle;
            setRotation(mAngle);
        }
        void setVitesse(double vitesse)
        {
            if(vitesse < vitesseMin())
                mVitesse = vitesseMin();
            else if(vitesse > vitesseMax())
                mVitesse = vitesseMax();
            else
                mVitesse = vitesse;
        }
        void setAcceleration(double acceleration)
        {
            mAcceleration = acceleration;
        }
        void setAccelerationMax(double accelerationMax)
        {
            mAccelerationMax = accelerationMax;
        }
        void setAngleIncrement(double angleIncrement)
        {
            mAngleIncrement = angleIncrement;
        }
        void setAngleMax(double angleMax)
        {
            mAngleMax = angleMax;
        }
        void setVitesseMin(double vitesseMin)
        {
            mVitesseMin = vitesseMin;
        }
        void setVitesseMax(double vitesseMax)
        {
            mVitesseMax = vitesseMax;
        }
        void setLooping(int i)
        {
            setPixmap(imagesMissile.at(i));
            mLooping = i;
        }

        //Méthodes liées à la class
        void refreshAuto();
        void refreshManuel();
        void refreshLooping();

    private:
        Xml *xml;
        Explosion *explosion;

        //Variables liées aux propriétés de la class
        QTime   temps;
        actionMissile action;
        typeMissile mType;
        explosionMissile typeExplosion;
        bool mIsCollision;

        QPointF mPosition;          //Position actuelle de l'avion (en x et y)
        double  mAngle;             //Angle d'inclinaison actuel de l'avion (en °)
        double  mAngleIncrement;    //Angle d'incrémentation pour chaque ms (en °)
        double  mAngleMax;          //Angle d'incrémentation maximum pour chaque ms (en °)
        double  mVitesse;           //Vitesse actuelle de l'avion (en pixel par ms)
        double  mVitesseMin;        //Vitesse minimum de l'avion (en pixel par ms)
        double  mVitesseMax;        //Vitesse maximum de l'avion (en pixel par ms)
        double  mAcceleration;      //Acceleration actuelle de l'avion (en pixel par ms pour chaque ms)
        double  mAccelerationMax;   //Acceleration maximum de l'avion (en pixel par ms pour chaque ms)
        int mLooping;

        //Animations liées aux propriétés de la class
        QParallelAnimationGroup *animationRoot;
        QPropertyAnimation *animationPosition;
        QPropertyAnimation *animationAngle;
        QPropertyAnimation *animationVitesse;

        QPropertyAnimation *animationLooping;

        QList<QPixmap> imagesMissile;

        QTimer *timer;

        //Autres déclarations
        QGraphicsView *view;
        QGraphicsScene *scene;
        Datas *datas;
};

#endif // MISSILE_H
