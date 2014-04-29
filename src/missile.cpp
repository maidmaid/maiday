#include "missile.h"
#include "avion.h"

#include "mainwindow.h"
#include <QDebug>


//-----------------------------------------------------------------------------------------------------------------------------
//Constructeur
Missile::Missile(QObject *parent,
                 QGraphicsScene *scene,
                 Datas *datas,
                 int idJoueurTireur,
                 int id,
                 QPointF position,
                 double angle,
                 typeMissile type)
    : QObject(parent), QGraphicsPixmapItem(0, scene)
{
    xml = new Xml(this, datas->moi->horloge());

    //Affectation des objets
    this->scene = scene;
    this->view = scene->views().at(0);
    this->datas = datas;
    this->mType = type;
    this->idJoueurTireur = idJoueurTireur;
    this->id = id;

    setVitesseMin(100. / 1000.);
    setVitesseMax(600. / 1000.);
    setAccelerationMax(500. / 1000000.);
    setAcceleration(0);
    setAngleIncrement(270. / 1000.);
    setAngleMax(270. / 1000.);

    setAngle(angle);
    setVitesse(500. / 1000.);
    setPosition(position);

    setZValue(-1);
    setScale(0.8);
    setTransformOriginPoint(boundingRect().center()); //Met le centre de rotation au centre de l'Item
    setCollision(false);

    //Images des missiles
    for(int i = 1; i <= 31; i++)
        imagesMissile.append(QString(i < 10 ? "img/missile/missile000%1.png" : "img/missile/missile00%1.png").arg(i));

    setZValue(0);

    //Animation de la position
    animationPosition = new QPropertyAnimation(this, "position");
    animationPosition->setDuration(25);

    //Animation de l'angle
    animationAngle = new QPropertyAnimation(this, "angle");
    animationAngle->setDuration(25);

    //Animation de la vitesse
    animationVitesse = new QPropertyAnimation(this, "vitesse");
    animationVitesse->setDuration(25);

    //Animation root
    animationRoot = new QParallelAnimationGroup(this);
    animationRoot->addAnimation(animationAngle);
    animationRoot->addAnimation(animationVitesse);
    animationRoot->addAnimation(animationPosition);

    connect(animationRoot, SIGNAL(finished()), this, SLOT(refreshAuto()));

    //Animation de looping
    animationLooping = new QPropertyAnimation(this, "looping");
    animationLooping->setDuration(500);

    connect(animationLooping, SIGNAL(finished()), this, SLOT(refreshLooping()));

    //Explosion
    explosion = new Explosion(this, scene);
    connect(explosion, SIGNAL(finished()), this, SLOT(deleteLater()));

    timer = new QTimer(this);
    switch(type)
    {
        case typeNormal:
            toutDroit();
            accelerer();
            break;

        case typeZigzag:
            connect(timer, SIGNAL(timeout()), this, SLOT(zigzag()));
            timer->start(150);
            setVitesse(vitesseMax());
            droite();
            break;

        case typeTournantDroite:
            droite();
            break;

        case typeTournantGauche:
            gauche();
            break;
    }

    temps.start();
    refreshAuto();
    refreshLooping();

    typeExplosion = explosionTimeout;
    QTimer::singleShot(1500, this, SLOT(exploser())); //capacité de x seconde
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
Missile::~Missile()
{
    //explosion->deleteLater();
    datas->supprimerMissile(this);
    emit explose();
}

//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Missile::refreshAuto()
{
    int ms = temps.elapsed();
    temps.restart();

    animationAngle->setStartValue(angle());
    animationAngle->setEndValue(angle() + angleIncrement() * ms);

    animationVitesse->setStartValue(vitesse());
    animationVitesse->setEndValue(vitesse() + acceleration() * ms);

    animationPosition->setStartValue(position());
    animationPosition->setEndValue(position() + QPointF(cos(rad(angle())) * vitesse() * ms,
                                                        sin(rad(angle())) * vitesse() * ms));

    animationRoot->start();
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Missile::refreshManuel()
{
    if(modeManuel)
    {
        animationAngle->setStartValue(angle());
        animationAngle->setEndValue(newAngle);

        animationVitesse->setStartValue(vitesse());
        animationVitesse->setEndValue(newVitesse);

        animationPosition->setStartValue(position());
        animationPosition->setEndValue(newPosition);

        animationRoot->start();

        modeManuel = false;
    }
    else
    {
        refreshAuto();
    }
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Missile::refreshLooping()
{
    animationLooping->setStartValue(0);
    animationLooping->setEndValue(imagesMissile.size() - 1);
    animationLooping->start();
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Missile::exploser()
{
    QPointF pos;

    setOpacity(0);
    switch(typeExplosion)
    {
        case explosionTimeout: pos = position(); explosion->demmarer(pos + boundingRect().center(), 0.2); break;
        case explosionCollision: pos = datas->chercherJoueur(idJoueurExplose)->avion()->centre(); explosion->demmarer(pos, 0.5); break;
        case explosionKill: pos = datas->chercherJoueur(idJoueurExplose)->avion()->centre(); explosion->demmarer(pos, 1); break;
    }
}
//-----------------------------------------------------------------------------------------------------------------------------
void Missile::zigzag()
{
    if(action == tourneAGauche)
        droite();
    else if(action == tourneADroite)
        gauche();
}
