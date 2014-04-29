#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <QDebug>
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>

class Explosion : public QObject, public QGraphicsPixmapItem
{
        Q_OBJECT
        Q_PROPERTY(double angle READ angle WRITE setAngle)
        Q_PROPERTY(double dilatation READ dilatation WRITE setDilatation)

    public:
        explicit Explosion(QObject *parent = 0, QGraphicsScene *scene = 0);

        void demmarer(QPointF position, double amplitude);
        double dilatation()
        {
            return mDilatation;
        }
        double angle()
        {
            return mAngle;
        }

    signals:
        void finished();

    private slots:
        void emitFinished()
        {
            emit finished();
        }
        void setAngle(double angle)
        {
            setRotation(angle);
            mAngle = angle;
        }
        void setDilatation(double dilatation)
        {
            setScale(dilatation);
            mDilatation = dilatation;
        }

    private:
        double mAngle;
        double mDilatation;

        QGraphicsScene *scene;
        QPropertyAnimation *animationExplosion1;
        QPropertyAnimation *animationExplosion2;
        QPropertyAnimation *animationExplosionTourner;
        QSequentialAnimationGroup *animationExplosion1_2;
        QParallelAnimationGroup *animationExplosionRoot;
};

#endif // EXPLOSION_H
