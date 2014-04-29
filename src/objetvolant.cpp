#include "objetvolant.h"
#define rad(ang) (M_PI * ang / 180.)

ObjetVolant::ObjetVolant(QObject *parent, QGraphicsScene *scene, int id) :
    QObject(parent), QGraphicsPixmapItem(0, scene)
{
    this->scene = scene;
    this->view = scene->views().at(0);
    setId(id);
    init();
}

void ObjetVolant::init()
{
    //Initialise les variables
    setZValue(1);
    setPilotage(manuel);
    setDessinerTrajectoire(false);
    creerImageParDefaut();
    setLooping(0);

    setVitesseAngulaireObjet(180.0 / 1000.0);
    setAccelerationObjet(100.0 / 1000000.0);
    setVitesseMinObjet(100.0 / 1000.0);
    setVitesseMaxObjet(300.0 / 1000.0);

    setPosition(QPointF(0, 0));
    setVitesseAngulaire(0);
    setAngle(vitesseAngulaire() * 0);
    setAcceleration(0);
    setVitesse(vitesseMinObjet());

    //Animation du looping
    animationLooping = new QPropertyAnimation(this, "looping");
    animationLooping->setDuration(500);
    animationLoopingRoot = new QSequentialAnimationGroup(this);
    animationLoopingRoot->addPause(1500); //Attend un moment avant de se retourner
    animationLoopingRoot->addAnimation(animationLooping);

    //Animation de la position
    animationPosition = new QPropertyAnimation(this, "position");
    animationPosition->setDuration(25);

    //Animation de l'angle
    setTransformOriginPoint(boundingRect().center()); //Met le centre de rotation au centre de l'Item
    animationAngle = new QPropertyAnimation(this, "angle");
    animationAngle->setDuration(25);

    //Animation de la vitesse
    animationVitesse = new QPropertyAnimation(this, "vitesse");
    animationVitesse->setDuration(25);

    //Animation principale des déplacements
    animationRoot = new QParallelAnimationGroup(this);
    animationRoot->addAnimation(animationAngle);
    animationRoot->addAnimation(animationVitesse);
    animationRoot->addAnimation(animationPosition);
    connect(animationRoot, SIGNAL(finished()), this, SLOT(seDeplacer()));

    temps.start();
    seDeplacer();
}

void ObjetVolant::creerImageParDefaut()
{
    //Dimension de l'image par défaut
    QSize size(20, 20);

    //Crée l'image par défaut
    double w = size.width() - 1.0;
    double h = size.height() - 1.0;

    QPixmap image(size);
    image.fill(Qt::transparent);

    QPainter p(&image);
    p.setBrush(QBrush(Qt::blue));
    p.drawEllipse(QPoint(w/2.0, h/2.0), w/2.0, h/2.0);
    p.setBrush(QBrush(Qt::green));
    p.drawEllipse(QPoint(w/1.5, h/4), w/10, h/10);
    p.drawLine(w/1.5, h/2.0, w, h/2.0);

    //Ajoute l'image par défaut
    mImages.append(image);
}

void ObjetVolant::seDeplacer()
{
    int ms;
    double distance;
    double angleRadian;
    QPointF newPosition;
    double newAngle;
    double newVitesse;

    //Calcule l'angle, la vitesse et la position
    ms = temps.elapsed();
    distance = vitesse() * ms;
    angleRadian = rad(angle());

    newAngle = angle() + vitesseAngulaire() * ms;
    newVitesse = vitesse() + acceleration() * ms;
    newPosition = position() + QPointF(cos(angleRadian) * distance,
                                       sin(angleRadian) * distance);

    //Déplace l'objet volant
    seDeplacer(newPosition, newAngle, newVitesse);
}

void ObjetVolant::seDeplacer(QPointF pointArrivee, double angleArrivee, double vitesseArrivee)
{
    //Prépare les animations de position, d'angle, et de vitesse
    animationPosition->setStartValue(position());
    animationPosition->setEndValue(pointArrivee);

    animationAngle->setStartValue(angle());
    animationAngle->setEndValue(angleArrivee);

    animationVitesse->setStartValue(vitesse());
    animationVitesse->setEndValue(vitesseArrivee);

    animationRoot->start();
    temps.restart();
}

void ObjetVolant::test(actionObjetVolant action, QPointF pointArrivee, double angleArrivee, double vitesseArrivee)
{
    animationRoot->stop();
    setAction(action);

    int ms;
    double distance;
    double angleRadian;
    QPointF newPosition;
    double newAngle;
    double newVitesse;

    ms = 25;
    distance = vitesse() * ms;
    angleRadian = rad(angle());

    newAngle = angle() + vitesseAngulaire() * ms;
    newVitesse = vitesse() + acceleration() * ms;
    newPosition = position() + QPointF(cos(angleRadian) * distance,
                                       sin(angleRadian) * distance);
}
