#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include <QObject>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include "datas.h"

class Affichage : public QObject
{
        Q_OBJECT
        Q_PROPERTY(QPointF position1 READ position1 WRITE setPosition1)
        Q_PROPERTY(QPointF position2 READ position2 WRITE setPosition2)
        Q_PROPERTY(QPointF position3 READ position3 WRITE setPosition3)

    public:
        explicit Affichage(QObject *parent = 0, QGraphicsScene *scene = 0, Datas *datas = 0);

    signals:

    public slots:
        QPointF position1()
        {
            return mPosition1;
        }
        void setPosition1(QPointF pos)
        {
            mPosition1 = pos;
            nuage1->setPos(pos);
        }
        QPointF position2()
        {
            return mPosition2;
        }
        void setPosition2(QPointF pos)
        {
            mPosition2 = pos;
            nuage2->setPos(pos);
        }
        QPointF position3()
        {
            return mPosition3;
        }
        void setPosition3(QPointF pos)
        {
            mPosition3 = pos;
            nuage3->setPos(pos);
        }

        void update();

    private:
        QGraphicsScene *scene;
        QGraphicsView *view;
        Datas *datas;

        QGraphicsPixmapItem *herbe;
        QGraphicsPixmapItem *arbre1;
        QGraphicsPixmapItem *arbre2;
        QGraphicsPixmapItem *arbre3;

        QGraphicsPixmapItem *montagne;

        QGraphicsPixmapItem *nuage1;
        QGraphicsPixmapItem *nuage2;
        QGraphicsPixmapItem *nuage3;

        QPointF mPosition1;
        QPointF mPosition2;
        QPointF mPosition3;

        QPropertyAnimation *animationNuage1;
        QPropertyAnimation *animationNuage2;
        QPropertyAnimation *animationNuage3;
        QParallelAnimationGroup *animationNuageRoot;

        QGraphicsPixmapItem *soleil;
};

#endif // AFFICHAGE_H
