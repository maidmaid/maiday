#ifndef AVION_H
#define AVION_H

//Class héritage
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QThread>

#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QTime>
#include <math.h>

#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QProgressBar>
#include <QGraphicsProxyWidget>

#include "xml.h"
#include "missile.h"

class Datas;

class Avion : public QObject, public QGraphicsPixmapItem
{
        Q_OBJECT
        //-------------------------------------------------------------
        Q_PROPERTY(QPointF position READ position WRITE setPosition)
        Q_PROPERTY(double angle READ angle WRITE setAngle)
        Q_PROPERTY(double vitesse READ vitesse WRITE setVitesse)
        //-------------------------------------------------------------
        Q_PROPERTY(int looping READ looping WRITE setLooping)
        //-------------------------------------------------------------
        Q_PROPERTY(int vie READ vie WRITE setVie)

    public:
        //enum
        enum typeAvion
        {
            perso,
            joueur,
            robot,
            suiveur
        };
        enum actionAvion
        {
            accelere,
            freine,
            plane,
            tourneADroite,
            tourneAGauche,
            tournePlus
        };
        enum sensAvion
        {
            sensDroite, // ->
            sensGauche  // <-
        };
        enum etatAvion
        {
            etatEnVie,
            etatMort
        };

        //Métodes liées aux propriétés de la class (méthodes de lecture)
        int id;

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
        int action()
        {
            return mAction;
        }
        void setTypeMissile(Missile::typeMissile typeMissile)
        {
            this->typeMissile = typeMissile;
        }
        int vie()
        {
            return mVie;
        }
        void setVie(int vie)
        {
            mVie = vie;
            barVie->setValue(vie);
        }
        int score()
        {
            return mScore;
        }
        void setScore(int score)
        {
            mScore = score;
            scoreItem->setPlainText(QString("+%1").arg(score));
        }
        int ajoutScore(int point)
        {
            setScore(score() + point);
        }
        void setManuel(QPointF newPosition, double newAngle, double newVitesse)
        {
            mNewPosition = newPosition;
            mNewAngle = newAngle;
            mNewVitesse = newVitesse;
            mManuel = true;
        }
        void setManuel(bool active)
        {
            mManuel = active;
        }
        bool manuel()
        {
            return mManuel;
        }
        QPointF newPosition()
        {
            return mNewPosition;
        }
        double newAngle()
        {
            return mNewAngle;
        }
        double newVitesse()
        {
            return mNewVitesse;
        }
        //Variable liées aux infos du joueur
        void setPseudo(QString pseudo)
        {
            pseudoItem->setPlainText(pseudo);
        }
        QPointF centre()
        {
            return position() + boundingRect().center();
        }
        QPointF newPositionCentre()
        {
            return newPosition() + boundingRect().center();
        }
        void setAction(actionAvion action)
        {
            switch(action)
            {
                case accelere:
                    accelerer();
                    break;
                case freine:
                    freiner();
                    break;
                case plane:
                    planer();
                    break;
                case tourneADroite:
                    droite();
                    break;
                case tourneAGauche:
                    gauche();
                    break;
                case tournePlus:
                    toutDroit();
                    break;
            }

            mAction = action;
        }
        void setVisible(bool visible)
        {
            QGraphicsPixmapItem::setVisible(visible);
            contenant->setVisible(visible);
        }

        void setDessinerTrajectoire(bool active)
        {
            if(type != perso)
            {
                //Activé: connecte les entrées sur les slots de dessin
                if(active)
                {
                    connect(this, SIGNAL(nouvelleActionServeur(actionAvion)), this, SLOT(dessinerAction(actionAvion)));
                    connect(this, SIGNAL(nouvelleTrajectoire(QPointF,QPointF)), this, SLOT(dessinerTrajectoire(QPointF,QPointF)));
                }
                else
                {
                    disconnect(this, SIGNAL(nouvelleActionServeur(actionAvion)), this, SLOT(dessinerAction(actionAvion)));
                    disconnect(this, SIGNAL(nouvelleTrajectoire(QPointF,QPointF)), this, SLOT(dessinerTrajectoire(QPointF,QPointF)));
                    clearItemTrajectoire();
                }
                mDessinerTrajectoire = active;
            }
        }
        bool isDessinerTrajectoire()
        {
            return mDessinerTrajectoire;
        }

        //Constructeur et destructeur
        explicit Avion(QObject *parent = 0, QGraphicsScene *scene = 0, Datas *datas = 0, typeAvion type = perso);
        ~Avion()
        {
            xml->deleteLater();
            contenant->deleteLater();
            contenantItem->deleteLater();
            pseudoItem->deleteLater();
            barVie->deleteLater();
            barVieItem->deleteLater();
            clearItemTrajectoire();
            if(type == joueur)
                 avionSuivi->deleteLater();
        }

        //Méthodes appellées par l'utilisateur pour diriger son avion
        void accelerer()
        {
            setAcceleration(accelerationMax());
            mAction = accelere;

            emit nouvelleActionServeur(mAction);
        }
        void freiner()
        {
            setAcceleration(-accelerationMax());
            mAction = freine;

            emit nouvelleActionServeur(mAction);
        }
        void planer()
        {
            setAcceleration(0);
            mAction = plane;

            emit nouvelleActionServeur(mAction);
        }
        void droite()
        {
            setAngleIncrement(angleMax());
            mAction = tourneADroite;

            emit nouvelleActionServeur(mAction);
        }
        void gauche()
        {
            setAngleIncrement(-angleMax());
            mAction = tourneAGauche;

            emit nouvelleActionServeur(mAction);
        }
        void toutDroit()
        {
            setAngleIncrement(0);
            mAction = tournePlus;

            emit nouvelleActionServeur(mAction);
        }
        void tirer(); //Tir et envoie au serveur
        void tirer(int idMissile, QPointF position, double angle, Missile::typeMissile type); //Recoit du serveur et tir
        void attaqueSpeciale();
        void repositionner(int temps, actionAvion action, QPointF position, double angle, double vitesse);

        //Méthodes pour démarrer/stopper l'avion lors d'une nouvelle/fin de partie
        void demarrer();
        void stopper();

    signals:
        //Permet d'envoyer au serveur via la class Client les infos sur l'avion
        void infoSortante(QString);
        void nouvelleActionServeur(actionAvion action);
        void nouvelleTrajectoire(QPointF pos1, QPointF pos2);
        void kill();
        void log(QString log);

    public slots:
        //Permet de recevoir des informations du serveur
        void infoEntrante(QString texte);
        etatAvion perdreVie()
        {
            setVie(vie() - 1);
            if(vie() <= 0)
            {
                etat = etatMort;
                //? BUG
                //emit kill();
                respawn();
            }
        }

    private slots:

        //Métodes liées aux propriétés de la class (méthodes d'écritures)
        void setPosition(QPointF position)
        {
            mPosition = position;
            setPos(mPosition);

            //Traverse les bords

            if (mPosition.x() < -boundingRect().width()) //à droite
                setX(scene->width());

            else if (mPosition.x() > scene->width()) //à gauche
                setX(-boundingRect().width());

            if (mPosition.y() > scene->height() - boundingRect().height()) //en bas
                setY(scene->height() - boundingRect().height());

            else if (mPosition.y() < 0) //en haut
                setY(0);

            mPosition = pos();

            contenantItem->setPos(mPosition);

            if(type == perso)
            {
                view->centerOn(this);
            }
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

            if(type == perso)

            //Looping
            if(animationLooping->state() == QPropertyAnimation::Stopped) //Si un looping n'est pas en cours
            {
                if(sens == sensDroite && mAngle > 135 && mAngle < 225)
                    faireLooping(sensGauche);
                else if(sens == sensGauche && ((mAngle > 285 && mAngle <= 360) || (mAngle > 0 && mAngle < 45)))
                    faireLooping(sensDroite);
                else
                    animationLoopingRoot->stop();
            }
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
            setPixmap(imagesAvion.at(i));
            mLooping = i;

            if(i == 0)
                sens = sensDroite;
            if(i == imagesAvion.size() - 1)
                sens = sensGauche;
        }
        void setKill(bool isKill)
        {
            setVisible(isKill);
            contenantItem->setVisible(isKill);

            if(isKill)
            {
                stopper();
                emit kill();
            }
            else
                demarrer();
        }

        void attenteRespawn()
        {
            setKill(true);
            QTimer::singleShot(3000, this, SLOT(respawn()));
        }

        void respawn()
        {
            stopper();

            setVie(10);
            setAngle(0);
            setVitesse(200. / 1000.);
            setPosition(QPointF(rand()%1080, rand()%1920));
            setAcceleration(0);
            setLooping(0);
            setAcceleration(0);

            toutDroit();
            planer();

            demarrer();
        }

        //Méthodes liées à la class
        void refreshAuto();
        void refreshManuel();
        void refreshRobot();
        void refreshSuiveur();

        void envoyerAction()
        {
            emit infoSortante(xml->ecrireAction(id, action(), position(), angle(), vitesse()));
        }

        //Trajectoire
        void dessinerAction(actionAvion action)
        {
            switch(action)
            {
                case Avion::accelere: dessinerPoint("V+", Qt::blue); break;
                case Avion::freine: dessinerPoint("V-", Qt::blue); break;
                case Avion::plane: dessinerPoint("V", Qt::blue); break;
                case Avion::tourneADroite: dessinerPoint("A>", Qt::red); break;
                case Avion::tourneAGauche: dessinerPoint("A<", Qt::red); break;
                case Avion::tournePlus: dessinerPoint("A", Qt::red); break;
            }
        }
        void dessinerTrajectoire(QPointF pos1, QPointF pos2)
        {
            pos1 += boundingRect().center();
            pos2 += boundingRect().center();

            //Dessigne la trajectoire
            lignes << scene->addLine(pos1.x(), pos1.y(), pos2.x(), pos2.y(), QPen(Qt::yellow));
            lignes.last()->setZValue(10);

            //Rend invisible si trop grande
            QPointF distance = pos1 - pos2;
            if(distance.x() > 1000 || distance.y() > 1000)
                lignes.last()->setOpacity(0);

            //Supprime les premières lignes
            if(lignes.size() >= 300)
            {
                lignes.removeFirst();
                lignes.first()->~QGraphicsItem();
            }
        }
        void dessinerPoint(QString text = "", QColor color = Qt::black)
        {
            //Quitte si point position et le meme que le point action précédent
            if(points.size() != 0)
            {
                if(points.last() == newPositionCentre() && text == "P")
                    return;
            }

            points << newPositionCentre();

            //Dessine le point
            QGraphicsEllipseItem *rond = scene->addEllipse(newPositionCentre().x() - 3, newPositionCentre().y() - 3, 6, 6, QPen(color));
            rond->setZValue(10);
            ronds << rond;

            //Marque le texte
            QGraphicsTextItem *textItem = scene->addText(text);
            textItem->setPos(newPositionCentre() + QPointF(3, 0));
            textItem->setZValue(10);
            texts << textItem;

            //Garde que les 20 derniers points. Si stop: supprime tout
            if(points.size() >= 20)
            {
                points.removeFirst();
                ronds.removeFirst();
                ronds.first()->~QGraphicsItem();
                texts.first()->~QGraphicsItem();
                texts.removeFirst();
            }
        }
        void clearItemTrajectoire()
        {
            //Supprime tous les points
            points.clear();

            //Supprime tous les ronds
            foreach (QGraphicsEllipseItem *r, ronds)
                r->~QGraphicsItem();
            ronds.clear();

            //Supprime tous les texts
            foreach (QGraphicsTextItem *t, texts)
                t->~QGraphicsItem();
            texts.clear();

            //Supprime toutes les lignes
            foreach (QGraphicsLineItem *l, lignes)
                l->~QGraphicsItem();
            lignes.clear();
        }

    private:
        Xml *xml;

        //Variables liées aux propriétés de la class
        typeAvion type;
        actionAvion mAction;
        sensAvion sens;
        etatAvion etat;
        Missile::typeMissile typeMissile;
        int idMissile;
        QTime   temps;
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
        int mVie;
        int mScore;
        QPointF mNewPosition;
        double mNewAngle;
        double mNewVitesse;
        bool mManuel;

        void faireLooping(sensAvion sens)
        {
            switch(sens)
            {
                case sensDroite:
                    animationLooping->setStartValue(looping());
                    animationLooping->setEndValue(0);
                    break;

                case sensGauche:
                    animationLooping->setStartValue(looping());
                    animationLooping->setEndValue(imagesAvion.size() - 1);
                    break;
            }

            animationLoopingRoot->start();
        }

        //Animations liées aux propriétés de la class
        QParallelAnimationGroup *animationRoot;
        QPropertyAnimation *animationPosition;
        QPropertyAnimation *animationAngle;
        QPropertyAnimation *animationVitesse;
        QPropertyAnimation *animationLooping;
        QSequentialAnimationGroup *animationLoopingRoot;

        //Avion
        QTimer *timerPosition;
        QTimer *timerRobot;
        QList<QPixmap> imagesAvion;
        QTimer *timerLooping;
        QGraphicsProxyWidget *contenantItem;
        QWidget *contenant;
        QGraphicsTextItem *pseudoItem;
        QGraphicsTextItem *scoreItem;
        QProgressBar *barVie;
        QGraphicsProxyWidget *barVieItem;

        //Autres déclarations
        QGraphicsView *view;
        QGraphicsScene *scene;
        Datas *datas;

        //Trajectoire
        bool mDessinerTrajectoire;
        QList <QPointF> points;
        QList <QGraphicsEllipseItem *> ronds;
        QList <QGraphicsTextItem *> texts;
        QList <QGraphicsLineItem *> lignes;

        //Suivi
        QGraphicsLineItem *ligneSuivi;

        //respawn
        QTimer *timerRespawn;

        Avion *avionSuivi;
};

#endif // AVION_H
