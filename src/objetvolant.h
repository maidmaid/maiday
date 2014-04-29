#ifndef OBJETVOLANT_H
#define OBJETVOLANT_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QTime>
#include <QPainter>
#include <math.h>

/**
 * \brief La classe ObjetVolant fournit un item qui se d�place dans une QGraphicsScene
 * \details Cette classe permet de d�placer un item gr�ce aux m�thodes gauche(), droite(), toutDroit().
 *          La vitesse de l'item peut �tre aussi contr�l�e gr�ce aux m�thodes freiner(), accelerer().
 *          La vitesse minimum et maximum ainsi que la vitesse de rotation de l'item peuvent �tre r�gl�es
 *          gr�ce aux m�thodes d'�critures.
 *          Il est envisageable aussi de g�rer un looping de l'objet volant lorsqu'il a la t�te en bas.
 */
class ObjetVolant : private QObject, private QGraphicsPixmapItem
{
        Q_OBJECT

        Q_PROPERTY(QPointF position READ position WRITE setPosition)
        Q_PROPERTY(double angle READ angle WRITE setAngle)
        Q_PROPERTY(double vitesse READ vitesse WRITE setVitesse)
        Q_PROPERTY(int looping READ looping WRITE setLooping)

    public:
        /**
         * \brief   Constructeur de la classe ObjetVolant
         * \param   parent  QObject parent
         * \param   scene   QGraphicsScene dans laquelle l'objet volant sera ins�rer
         * \param   id      Cl� primaire
         */
        explicit ObjetVolant(QObject *parent = 0,
                             QGraphicsScene *scene = 0,
                             int id = 0);

        /**
         * \brief Enum�ration de l'action en cours
         */
        enum actionObjetVolant
        {
            accelere,       /**< L'objet volant est en train d'accel�rer*/
            freine,         /**< L'objet volant est en train de freiner*/
            plane,          /**< L'objet volant est en train de planer*/
            tourneADroite,  /**< L'objet volant est en train de tourner � droite*/
            tourneAGauche,  /**< L'objet volant est en train de tourner � gauche*/
            tournePlus      /**< L'objet volant est en train d'aller tout droit*/
        };

        /**
         * \brief Enum�ration des types de pilotage de l'objet volant
         */
        enum pilotageObjetVolant
        {
            manuel,     /**< L'objet volant est pilot� manuellement, � l'aide du clavier*/
            distant,    /**< L'objet volant est pilot� � distance, par un autre joueur sur le r�seau*/
            automatique /**< L'objet volant est pilot� automatique, par un robot*/
        };

        //Propri�t�s
        /**
         * \brief Attribue un id (num�ro unique)
         * \see id()
         */
        void setId(int id)
        {
            mId = id;
        }

        /**
         * \brief Retourne l'id (num�ro unique)
         * \see setId()
         */
        int id()
        {
            return mId;
        }

        /**
         * \brief Retourne la position en cours en x et y [P(x, y)]
         * \see setPosition()
         */
        QPointF position()
        {
            return mPosition;
        }

        /**
         * \brief Retourne l'angle en cours en degr�s [�]
         * \see setAngle()
         */
        double angle()
        {
            return mAngle;
        }

        /**
         * \brief Retourne la vitesse en cours en pixel par milliseconde [px/ms]
         * \see setVitesse()
         */
        double vitesse()
        {
            return mVitesse;
        }

        /**
         * \brief Retourne l'acc�l�ration en cours en pixel par milliseconde par milliseconde [px/ms^2]
         * \see setAcceleration()
         */
        double acceleration()
        {
            return mAcceleration;
        }

        /**
         * \brief Attribue l'acc�l�ration de l'objet en pixel par milliseconde par milliseconde [px/ms^2]
         * \see accelerationObjet()
         */
        void setAccelerationObjet(double accelerationObjet)
        {
            mAccelerationObjet = accelerationObjet;
        }

        /**
         * \brief Retourne l'acceleration de l'objet en pixel par milliseconde par milliseconde [px/ms^2]
         * \see setAccelerationObjet()
         */
        double accelerationObjet()
        {
            return mAccelerationObjet;
        }

        /**
         * \brief Retourne la vitesse angulaire (vitesse de rotation) en cours de l'objet en degr�s par milliseconde [�/ms]
         * \see setAccelerationObjet()
         */
        double vitesseAngulaire()
        {
            return mVitesseAngulaire;
        }

        /**
         * \brief Attribue la vitesse angulaire (vitesse de rotation) de l'objet en degr�s par milliseconde [�/ms]
         * \see vitesseAngulaireObjet()
         */
        void setVitesseAngulaireObjet(double vitesseAngulaireObjet)
        {
            mVitesseAngulaireObjet = vitesseAngulaireObjet;
        }

        /**
         * \brief Retourne la vitesse angulaire (vitesse de rotation) de l'objet en degr�s par milliseconde [�/ms]
         * \see setVitesseAngulaireObjet()
         */
        double vitesseAngulaireObjet()
        {
            return mVitesseAngulaireObjet;
        }

        /**
         * \brief Attribue la vitesse minimum de l'objet en pixels par milliseconde [px/ms]
         * \see vitesseMinObjet()
         */
        void setVitesseMinObjet(double vitesseMinObjet)
        {
            mVitesseMinObjet = vitesseMinObjet;
        }

        /**
         * \brief Retourne la vitesse minimum de l'objet en pixels par milliseconde [px/ms]
         * \see setVitesseMinObjet()
         */
        double vitesseMinObjet()
        {
            return mVitesseMinObjet;
        }

        /**
         * \brief Attribue la vitesse maximum de l'objet en pixels par milliseconde [px/ms]
         * \see vitesseMaxObjet()
         */
        void setVitesseMaxObjet(double vitesseMaxObjet)
        {
            mVitesseMaxObjet = vitesseMaxObjet;
        }

        /**
         * \brief Retourne la vitesse maximum de l'objet en pixels par milliseconde [px/ms]
         * \see setVitesseMaxObjet()
         */
        double vitesseMaxObjet()
        {
            return mVitesseMaxObjet;
        }

        /**
         * \brief Retourne le num�ro de l'image dans l'animation du looping
         * \see setLooping()
         */
        int looping()
        {
            return mLooping;
        }

        /**
         * \brief Attribue l'action
         * \see actionObjetVolant, setAction()
         */
        void setAction(actionObjetVolant action)
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

        /**
         * \brief Retourne l'action en cours
         * \see actionObjetVolant, setAction()
         */
        actionObjetVolant action()
        {
            return mAction;
        }

        /**
         * \brief Retourne le type de pilotage de l'objet
         * \see pilotageObjetVolant, setPilotage()
         */
        pilotageObjetVolant pilotage()
        {
            return mPilotage;
        }

        //M�thodes appell�es par l'utilisateur pour diriger l'objet volant
        /**
         * \brief Accel�re l'objet volant jusqu'� sa vitesse maximale
         */
        void accelerer()
        {
            setAcceleration(accelerationObjet());
        }

        /**
         * \brief Freine l'objet volant jusqu'� sa vitesse minimale
         */
        void freiner()
        {
            setAcceleration(-accelerationObjet());
        }

        /**
         * \brief Maintient la vitesse actuelle de l'objet volant
         */
        void planer()
        {
            setAcceleration(0);
        }

        /**
         * \brief Fait tourner � droite l'objet volant
         */
        void droite()
        {
            setVitesseAngulaire(vitesseAngulaireObjet());
        }

        /**
         * \brief Fait tourner � gauche l'objet volant
         */
        void gauche()
        {
            setVitesseAngulaire(-vitesseAngulaireObjet());
        }

        /**
         * \brief Fait prendre une tajectoire rectiligne � l'objet volant
         */
        void toutDroit()
        {
            setVitesseAngulaire(0);
        }

    signals:
        void infoSortante(QString);

    public slots:

    private slots:
        void init();
        void creerImageParDefaut();

        //M�todes li�es aux propri�t�s de la class (m�thodes d'�critures)
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
            if(vitesse < vitesseMinObjet())
                mVitesse = vitesseMinObjet();
            else if(vitesse > vitesseMaxObjet())
                mVitesse = vitesseMaxObjet();
            else
                mVitesse = vitesse;
        }
        void setAcceleration(double acceleration)
        {
            mAcceleration = acceleration;
        }
        void setVitesseAngulaire(double vitesseAngulaire)
        {
            mVitesseAngulaire = vitesseAngulaire;
        }
        void setLooping(int i)
        {
            setPixmap(mImages.at(i));
            mLooping = i;
        }

        /**
        * \todo Finir le code
        */
        void setPilotage(pilotageObjetVolant pilotage)
        {
            //            switch(pilotage)
            //            {
            //                case manuel:
            //                    connect(animationRoot, SIGNAL(finished()), this, SLOT(seDeplacer()));
            //                    break;
            //                case distant:
            //                    connect(animationRoot, SIGNAL(finished()), this, SLOT(seDeplacer()));
            //                    break;
            //                case automatique:
            //                    break;

            //            }
            mPilotage = pilotage;
        }

        /**
        * \todo Cr�er un QPixmap si pas d'image
        */
        void setImages(QList<QPixmap> images)
        {
            if(!images.isEmpty())
            {
                mImages = images;
                setLooping(0);
            }
            else
            {
                creerImageParDefaut();
            }
        }

        /**
        * \todo A r�fl�chir si bonne id�e
        */
        void setImages(QString type)
        {
            QList<QPixmap> images;

            if(type == "avion")
            {
                for(int i = 1; i <= 31; i++)
                {
                    if(i < 10)
                        images.append(QString("img/avion/avion000%1.png").arg(i));
                    else
                        images.append(QString("img/avion/avion00%1.png").arg(i));
                }
            }

            setImages(images);
        }

        /**
         * \todo Coder la fonction
         */
        void setDessinerTrajectoire(bool dessinerTrajectoire)
        {

        }

        void seDeplacer();
        void seDeplacer(QPointF pointArrivee, double angleArrivee, double vitesseArrivee);
        void test(actionObjetVolant action, QPointF pointArrivee, double angleArrivee, double vitesseArrivee);

    private:

        //Propri�t�s priv�es
        int     mId;
        QPointF mPosition;          //Position actuelle de l'avion (en x et y)
        double  mAngle;             //Angle d'inclinaison actuel de l'avion (en �)
        double  mVitesseAngulaire;    //Angle d'incr�mentation pour chaque ms (en �)
        double  mVitesseAngulaireObjet;          //Angle d'incr�mentation maximum pour chaque ms (en �)
        double  mVitesse;           //Vitesse actuelle de l'avion (en pixel par ms)
        double  mVitesseMinObjet;        //Vitesse minimum de l'avion (en pixel par ms)
        double  mVitesseMaxObjet;        //Vitesse maximum de l'avion (en pixel par ms)
        double  mAcceleration;      //Acceleration actuelle de l'avion (en pixel par ms pour chaque ms)
        double  mAccelerationObjet;   //Acceleration maximum de l'avion (en pixel par ms pour chaque ms)
        int     mLooping;
        bool    mDessinerTrajectoire;
        actionObjetVolant mAction;
        pilotageObjetVolant mPilotage;
        QList<QPixmap> mImages;
        QTime temps;

        //Animations li�es aux propri�t�s de la class
        QParallelAnimationGroup *animationRoot;
        QPropertyAnimation *animationPosition;
        QPropertyAnimation *animationAngle;
        QPropertyAnimation *animationVitesse;
        QPropertyAnimation *animationLooping;
        QSequentialAnimationGroup *animationLoopingRoot;

        //Autres d�clarations
        QGraphicsView *view;
        QGraphicsScene *scene;
};

#endif // OBJETVOLANT_H
