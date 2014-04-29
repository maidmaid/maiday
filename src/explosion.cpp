#include "explosion.h"

Explosion::Explosion(QObject *parent, QGraphicsScene *scene) :
    QObject(parent), QGraphicsPixmapItem(0, scene)
{
    int ms = 200;

    this->scene = scene;

    setOpacity(0);
    setPixmap(QPixmap("img/fond/explosion.png"));
    setZValue(3);
    setTransformOriginPoint(boundingRect().center());

    animationExplosion1 = new QPropertyAnimation(this, "dilatation");
    animationExplosion2 = new QPropertyAnimation(this, "dilatation");
    animationExplosionTourner = new QPropertyAnimation(this, "angle");
    animationExplosion1_2 = new QSequentialAnimationGroup(this);
    animationExplosionRoot = new QParallelAnimationGroup(this);

    animationExplosion1->setStartValue(0.0);
    animationExplosion1->setEndValue(1.0);
    animationExplosion1->setEasingCurve(QEasingCurve::Linear);
    animationExplosion1->setDuration(ms);

    animationExplosion2->setStartValue(1);
    animationExplosion2->setEndValue(0);
    animationExplosion2->setEasingCurve(QEasingCurve::Linear);
    animationExplosion2->setDuration(ms);

    animationExplosion1_2->addAnimation(animationExplosion1);
    animationExplosion1_2->addAnimation(animationExplosion2);

    animationExplosionTourner->setStartValue(0);
    animationExplosionTourner->setEndValue(180);
    animationExplosionTourner->setDuration(ms * 2);

    animationExplosionRoot->addAnimation(animationExplosion1_2);
    animationExplosionRoot->addAnimation(animationExplosionTourner);

    connect(animationExplosionRoot, SIGNAL(finished()), this, SLOT(emitFinished()));
    connect(animationExplosionRoot, SIGNAL(finished()), this, SLOT(deleteLater()));
}

void Explosion::demmarer(QPointF position, double amplitude)
{
    animationExplosion1->setEndValue(amplitude);
    animationExplosion2->setStartValue(amplitude);

    setPos(position - boundingRect().center());
    setOpacity(1);
    animationExplosionRoot->start();
}
