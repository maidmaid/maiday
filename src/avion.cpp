#include "avion.h"
#include "mainwindow.h"
#include <QDebug>

#include <QLabel>

//-----------------------------------------------------------------------------------------------------------------------------
//Constructeur
Avion::Avion(QObject *parent, QGraphicsScene *scene, Datas *datas, typeAvion type)
    : QObject(parent), QGraphicsPixmapItem(0, scene)
{
    //Formatage XML
    xml = new Xml(this, datas->moi->horloge());

    //Affectation des objets
    this->scene = scene;
    this->view = scene->views().at(0);
    this->datas = datas;
    this->type = type;

    //Initialisation
    id = -1;
    setZValue(0);
    typeMissile = Missile::typeNormal;
    idMissile = 0;

    //Item contenant
    contenant = new QWidget();
    contenant->setGeometry(0, 0, 100, 40);
    contenant->setStyleSheet("background-color: transparent");
    contenantItem = scene->addWidget(contenant);

    //Pseudo
    pseudoItem = new QGraphicsTextItem("", contenantItem, scene);
    pseudoItem->setPos(0, -5);
    pseudoItem->setFont(QFont("Helvetica", 8, QFont::Bold));
    setPseudo("inconnu");

    //Score
    scoreItem = new QGraphicsTextItem("", contenantItem, scene);
    scoreItem->setPos(0, 22);
    scoreItem->setFont(QFont("Helvetica", 8, QFont::Bold));

    //Vie
    barVie = new QProgressBar();
    barVie->resize(100, 10);
    barVie->setTextVisible(false);
    barVie->setRange(0, 10);
    barVie->setValue(10);
    barVie->setStyleSheet("QProgressBar {border: 2px solid grey; border-radius: 5px; background:transparent;} QProgressBar:horizontal {background:black} QProgressBar::chunk {background-color: green;}");
    barVieItem = scene->addWidget(barVie);
    barVieItem->setParentItem(contenantItem);
    barVieItem->setPos(0, 15);
    barVieItem->setZValue(0);

    //Images de l'avion
    for(int i = 1; i <= 31; i++)
        imagesAvion.append(QString(i < 10 ? "img/avion/avion000%1.png" : "img/avion/avion00%1.png").arg(i));

    setZValue(1);

    //Animation du looping
    animationLooping = new QPropertyAnimation(this, "looping");
    animationLooping->setDuration(500);
    //animationLooping->setEasingCurve(QEasingCurve::InOutQuad);
    animationLoopingRoot = new QSequentialAnimationGroup(this);
    animationLoopingRoot->addPause(1500); //Attends un moment avant de se retourner
    animationLoopingRoot->addAnimation(animationLooping);
    setLooping(0);

    //Animation de la position
    animationPosition = new QPropertyAnimation(this, "position");
    animationPosition->setDuration(25);

    //Animation de l'angle
    animationAngle = new QPropertyAnimation(this, "angle");
    animationAngle->setDuration(25);
    setAngleMax(180. / 1000.);
    setTransformOriginPoint(boundingRect().center()); //Met le centre de rotation au centre de l'Item

    //Animation de la vitesse
    animationVitesse = new QPropertyAnimation(this, "vitesse");
    animationVitesse->setDuration(25);
    setAccelerationMax(500. / 1000000.);
    setVitesseMin(100. / 1000.);
    setVitesseMax(300. / 1000.);

    //Animation root
    animationRoot = new QParallelAnimationGroup(this);
    animationRoot->addAnimation(animationAngle);
    animationRoot->addAnimation(animationVitesse);
    animationRoot->addAnimation(animationPosition);

    //Dessiner la trajectoire
    setDessinerTrajectoire(false);
    mDessinerTrajectoire = false;

    //Catégorie d'avion
    switch(type)
    {
        case perso:
            timerPosition = new QTimer(this);
            connect(timerPosition, SIGNAL(timeout()), this, SLOT(envoyerAction()));
            timerPosition->start(500);

            connect(animationRoot, SIGNAL(finished()), this, SLOT(refreshAuto()));
            break;

        case joueur:

            //!
            //setManuel(false);
            //connect(animationRoot, SIGNAL(finished()), this, SLOT(refreshManuel()));

            //!! suiveur
            avionSuivi = new Avion(parent, scene, datas, suiveur);
ligneSuivi = scene->addLine(x(), y(), avionSuivi->x(), avionSuivi->y(), QPen(Qt::red));
//            avionSuivi->setVisible(false);
//            animationPosition->setDuration(50);
//            animationAngle->setDuration(50);
//            animationVitesse->setDuration(50);
            connect(animationRoot, SIGNAL(finished()), this, SLOT(refreshSuiveur()));
            break;

        case robot:
            timerRobot = new QTimer(this);
            connect(timerPosition, SIGNAL(timeout()), this, SLOT(refreshRobot()));
            timerRobot->start(1000);
            break;

        case suiveur:
            connect(animationRoot, SIGNAL(finished()), this, SLOT(refreshAuto()));
            break;
    }

    //Initialise
    connect(this, SIGNAL(nouvelleActionServeur(actionAvion)), this, SLOT(envoyerAction()));
    connect(this, SIGNAL(kill()), this, SLOT(attenteRespawn()));
    respawn();
    setScore(0);
}
//-----------------------------------------------------------------------------------------------------------------------------
//Demarre l'avion
void Avion::demarrer()
{
    temps.start();

    switch(type)
    {
        case perso:
            temps.start();
            refreshAuto();
            break;
        case joueur:
            //!
            //setManuel(false);
            //refreshManuel();
            avionSuivi->demarrer();
            refreshSuiveur();
            break;
        case robot:
            break;
        case suiveur:
            temps.start();
            refreshAuto();
            break;
    }
}
//-----------------------------------------------------------------------------------------------------------------------------
//Stoppe l'avion
void Avion::stopper()
{
    animationRoot->stop();
}
//-----------------------------------------------------------------------------------------------------------------------------
//
void Avion::infoEntrante(QString texte)
{

}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Avion::refreshAuto()
{
    int ms = temps.elapsed();
    temps.restart();

    //Animation de l'angle
    animationAngle->setStartValue(angle());
    animationAngle->setEndValue(angle() + angleIncrement() * ms);

    //Animation de de la vitesse
    animationVitesse->setStartValue(vitesse());
    animationVitesse->setEndValue(vitesse() + acceleration() * ms);

    //Animation de la position
    animationPosition->setStartValue(position());
    animationPosition->setEndValue(position() + QPointF(cos(rad(angle())) * vitesse() * ms,
                                                        sin(rad(angle())) * vitesse() * ms));

    animationRoot->start();

    /*
    qDebug() << "Angle: " << angle() << "°";
    qDebug() << "Vitesse: " << vitesse() * 1000. << " px/s";
    qDebug() << "Accelération: " << acceleration() * 1000;
    qDebug() << "Position: " << animationPosition->endValue();
    qDebug() << "Delta t: " << ms << "ms";
    */
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Avion::refreshManuel()
{
    QPointF pos2;

    //Données du serveur
    if(manuel())
    {
        temps.restart();

        //Animation de l'angle
        animationAngle->setStartValue(angle());
        animationAngle->setEndValue(newAngle());

        //Animation de la vitesse
        animationVitesse->setStartValue(vitesse());
        animationVitesse->setEndValue(newVitesse());

        //Animation de la position
        animationPosition->setStartValue(position());
        animationPosition->setEndValue(newPosition());

        pos2 = newPosition();

        //!
        //nouvellePositionServeur(newPosition());

        setManuel(false);
    }
    else //Données auto
    {
        int ms = temps.elapsed();
        temps.restart();

        //Animation de l'angle
        animationAngle->setStartValue(angle());
        animationAngle->setEndValue(angle() + angleIncrement() * (ms-2));

        //Animation de la vitesse
        animationVitesse->setStartValue(vitesse());
        animationVitesse->setEndValue(vitesse() + acceleration() * (ms-2));

        pos2 = position() + QPointF(cos(rad(angle())) * vitesse() * (ms-2),
                                    sin(rad(angle())) * vitesse() * (ms-2));

        //Animation de la position
        animationPosition->setStartValue(position());
        animationPosition->setEndValue(pos2);

    }

    QPointF pos1 = position();

    animationRoot->start();

    emit nouvelleTrajectoire(pos1, pos2);
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Avion::refreshRobot()
{
    switch(rand()%6)
    {
        case Avion::accelere: accelerer(); break;
        case Avion::freine: freiner(); break;
        case Avion::plane: planer(); break;
        case Avion::tourneADroite: droite(); break;
        case Avion::tourneAGauche: gauche(); break;
        case Avion::tournePlus: toutDroit(); break;
    }
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Avion::tirer() //Tir et envoie au serveur
{
    Missile *missile = new Missile(this, scene, datas, id, ++idMissile, position() + boundingRect().center(), angle(), typeMissile);
    connect(this, SIGNAL(destroyed()), missile, SLOT(deleteLater()));

    infoSortante(xml->ecrireTir(missile));
    datas->missiles.append(missile);
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Avion::tirer(int idMissile, QPointF position, double angle, Missile::typeMissile type) //Recoit du serveur et tir
{
    Missile *missile = new Missile(this, scene, datas, id, idMissile, position, angle, type);
    connect(this, SIGNAL(destroyed()), missile, SLOT(deleteLater()));
    datas->missiles.append(missile);
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Avion::attaqueSpeciale()
{
    for(int i = 0; i < 360; i = i + 20)
    {
        Missile *missile1 = new Missile(this, scene, datas, id, ++idMissile, position() + boundingRect().center(), i, Missile::typeTournantDroite);
        connect(this, SIGNAL(destroyed()), missile1, SLOT(deleteLater()));

        infoSortante(xml->ecrireTir(missile1));
        datas->missiles.append(missile1);

        Missile *missile2 = new Missile(this, scene, datas, id, ++idMissile, position() + boundingRect().center(), i, Missile::typeTournantGauche);
        connect(this, SIGNAL(destroyed()), missile2, SLOT(deleteLater()));

        infoSortante(xml->ecrireTir(missile2));
        datas->missiles.append(missile2);
    }
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Avion::repositionner(int temps, actionAvion action, QPointF position, double angle, double vitesse)
{
    avionSuivi->animationRoot->stop();

    int ms;
    double vitesseFinal;
    double distance;
    double angleRadian;
    QPointF newPosition;
    double newAngle;
    double newVitesse;

    avionSuivi->setAction(action);
    ms = datas->moi->horloge()->heureServeur() - temps + 25;
    vitesseFinal = vitesse + avionSuivi->acceleration() * ms;
    angleRadian = rad(angle);

    //Calcule la distance, en ne dépassant pas la vitesse maximale
    if(vitesseFinal <= avionSuivi->vitesseMax())
    {
        distance = 0.5 * avionSuivi->acceleration() * ms * ms + vitesse * ms; //MRUA: d = 0.5at^2 + vt
    }
    else
    {
        int msAcceleration = (avionSuivi->vitesseMax() - vitesse) / avionSuivi->acceleration(); //Temps de l'accéleration
        int msVitesseMax = ms - msAcceleration; //Temps du trajet avec la vitesse max, juste après l'accéleration
        distance = 0.5 * avionSuivi->acceleration() * msAcceleration * msAcceleration +
                   vitesse * ms +
                   avionSuivi->vitesseMax() * msVitesseMax ; //MRUA: d = 0.5at^2 + vt
    }

    newAngle = angle + avionSuivi->angleIncrement() * ms;
    newVitesse = (vitesse + avionSuivi->acceleration() * ms > avionSuivi->vitesseMax()) ? avionSuivi->vitesseMax() : vitesse + avionSuivi->acceleration() * ms;
    newPosition = position + QPointF(cos(angleRadian) * distance,
                                       sin(angleRadian) * distance);

    avionSuivi->animationPosition->setStartValue(avionSuivi->position());
    avionSuivi->animationPosition->setEndValue(newPosition);
    avionSuivi->animationAngle->setStartValue(avionSuivi->angle());
    avionSuivi->animationAngle->setEndValue(newAngle);
    avionSuivi->animationVitesse->setStartValue(avionSuivi->vitesse());
    avionSuivi->animationVitesse->setEndValue(newVitesse);

    avionSuivi->animationRoot->start();

    this->temps.restart();
}

void Avion::refreshSuiveur()
{
//    animationRoot->stop();
//    animationPosition->setStartValue(position());
//    animationPosition->setEndValue(avionSuivi->position());
//    animationAngle->setStartValue(angle());
//    animationAngle->setEndValue(avionSuivi->angle());
//    animationVitesse->setStartValue(vitesse());
//    animationVitesse->setEndValue(avionSuivi->vitesse());
//    animationRoot->start();

//    double angleTemp;
//    double distance;
//    double distanceX;
//    double distanceY;
//    double resultat;
//    double incertitude;
//    //...

//    // Distance
//    //distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
//    distance = abs(QPointF(avionSuivi->position() - position()).manhattanLength());

//    if(distance > 500)
//    {
//        setPosition(avionSuivi->position());
//        setAngle(avionSuivi->angle());
//        //setVitesse(avionSuivi->vitesse());
//        setAction((actionAvion)avionSuivi->action());
//    }

//    distanceX = x() - avionSuivi->x();
//    distanceY = y() - avionSuivi->y();
//    angleTemp = deg(atan(distanceY/distanceX));

//    if(distanceX > 0 && distanceY > 0)
//        resultat = angleTemp;
//    else if(distanceX > 0 && distanceY < 0)
//        resultat = 360 + angleTemp;
//    else if((distanceX < 0 && distanceY > 0) || (distanceX < 0 && distanceY < 0))
//        resultat = 180 + angleTemp;
//    else
//        resultat = 0;

//    //resultat = abs(resultat - angle());

//    if(resultat >= 0 && resultat <= 180)
//    {
//        droite();
//    }
//    else if(resultat > 180 && resultat <= 360)
//    {
//        gauche();
//    }

//    qDebug() << "_____________________________";
//    qDebug() << "Distance X: " << (int)distanceX;
//    qDebug() << "Distance Y: " << (int)distanceY;
//    qDebug() << "Angle atan: " << (int)angleTemp;
//    qDebug() << "Angle     : " << (int)angle();
//    qDebug() << "Resultat  : " << (int)resultat;

//    refreshAuto();

//    ligneSuivi->setLine(QLineF(centre(), avionSuivi->centre()));

    int ms = temps.elapsed();
    temps.restart();

    int angleIndex = avionSuivi->angle() - angle() > 0 ? 1 : -1;
    int vitesseIndex = avionSuivi->vitesse() - vitesse() > 0 ? 1 : -1;

    QPointF diff = QPointF(avionSuivi->position() - position());
    diff = QPointF(diff.x()/2., diff.y()/2.);

    //Animation de l'angle
    animationAngle->setStartValue(angle());
    animationAngle->setEndValue(avionSuivi->angle());

    //Animation de de la vitesse
    animationVitesse->setStartValue(vitesse());
    animationVitesse->setEndValue(avionSuivi->vitesse());

    double distance = avionSuivi->vitesse() * ms;


    //Animation de la position
    animationPosition->setStartValue(position());
    animationPosition->setEndValue(QPointF(position() + diff));

    animationRoot->start();
}


