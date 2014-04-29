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
 * \brief La classe ObjetVolant fournit un item qui se déplace dans une QGraphicsScene
 * \details Cette classe permet de déplacer un item grâce aux méthodes gauche(), droite(), toutDroit().
 *          La vitesse de l'item peut être aussi contrôlée grâce aux méthodes freiner(), accelerer().
 *          La vitesse minimum et maximum ainsi que la vitesse de rotation de l'item peuvent être réglées
 *          grâce aux méthodes d'écritures.
 *          Il est envisageable aussi de gérer un looping de l'objet volant lorsqu'il a la tête en bas.
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
         * \param   scene   QGraphicsScene dans laquelle l'objet volant sera insérer
         * \param   id      Clé primaire
         */
        explicit ObjetVolant(QObject *parent = 0,
                             QGraphicsScene *scene = 0,
                             int id = 0);

        /**
         * \brief Enumération de l'action en cours
         */
        enum actionObjetVolant
        {
            accelere,       /**< L'objet volant est en train d'accelérer*/
            freine,         /**< L'objet volant est en train de freiner*/
            plane,          /**< L'objet volant est en train de planer*/
            tourneADroite,  /**< L'objet volant est en train de tourner à droite*/
            tourneAGauche,  /**< L'objet volant est en train de tourner à gauche*/
            tournePlus      /**< L'objet volant est en train d'aller tout droit*/
        };

        /**
         * \brief Enumération des types de pilotage de l'objet volant
         */
        enum pilotageObjetVolant
        {
            manuel,     /**< L'objet volant est piloté manuellement, à l'aide du clavier*/
            distant,    /**< L'objet volant est piloté à distance, par un autre joueur sur le réseau*/
            automatique /**< L'objet volant est piloté automatique, par un robot*/
        };

        //Propriétés
        /**
         * \brief Attribue un id (numéro unique)
         * \see id()
         */
        void setId(int id)
        {
            mId = id;
        }

        /**
         * \brief Retourne l'id (numéro unique)
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
         * \brief Retourne l'angle en cours en degrés [°]
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
         * \brief Retourne l'accélération en cours en pixel par milliseconde par milliseconde [px/ms^2]
         * \see setAcceleration()
         */
        double acceleration()
        {
            return mAcceleration;
        }

        /**
         * \brief Attribue l'accélération de l'objet en pixel par milliseconde par milliseconde [px/ms^2]
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
         * \brief Retourne la vitesse angulaire (vitesse de rotation) en cours de l'objet en degrés par milliseconde [°/ms]
         * \see setAccelerationObjet()
         */
        double vitesseAngulaire()
        {
            return mVitesseAngulaire;
        }

        /**
         * \brief Attribue la vitesse angulaire (vitesse de rotation) de l'objet en degrés par milliseconde [°/ms]
         * \see vitesseAngulaireObjet()
         */
        void setVitesseAngulaireObjet(double vitesseAngulaireObjet)
        {
            mVitesseAngulaireObjet = vitesseAngulaireObjet;
        }

        /**
         * \brief Retourne la vitesse angulaire (vitesse de rotation) de l'objet en degrés par milliseconde [°/ms]
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
         * \brief Retourne le numéro de l'image dans l'animation du looping
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

        //Méthodes appellées par l'utilisateur pour diriger l'objet volant
        /**
         * \brief Accelère l'objet volant jusqu'à sa vitesse maximale
         */
        void accelerer()
        {
            setAcceleration(accelerationObjet());
        }

        /**
         * \brief Freine l'objet volant jusqu'à sa vitesse minimale
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
         * \brief Fait tourner à droite l'objet volant
         */
        void droite()
        {
            setVitesseAngulaire(vitesseAngulaireObjet());
        }

        /**
         * \brief Fait tourner à gauche l'objet volant
         */
        void gauche()
        {
            setVitesseAngulaire(-vitesseAngulaireObjet());
        }

        /**
         * \brief Fait prendre une tajectoire rectiligne à l'objet volant
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
        * \todo Créer un QPixmap si pas d'image
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
        * \todo A réfléchir si bonne idée
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

        //Propriétés privées
        int     mId;
        QPointF mPosition;          //Position actuelle de l'avion (en x et y)
        double  mAngle;             //Angle d'inclinaison actuel de l'avion (en °)
        double  mVitesseAngulaire;    //Angle d'incrémentation pour chaque ms (en °)
        double  mVitesseAngulaireObjet;          //Angle d'incrémentation maximum pour chaque ms (en °)
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

        //Animations liées aux propriétés de la class
        QParallelAnimationGroup *animationRoot;
        QPropertyAnimation *animationPosition;
        QPropertyAnimation *animationAngle;
        QPropertyAnimation *animationVitesse;
        QPropertyAnimation *animationLooping;
        QSequentialAnimationGroup *animationLoopingRoot;

        //Autres déclarations
        QGraphicsView *view;
        QGraphicsScene *scene;
};

#endif // OBJETVOLANT_H
