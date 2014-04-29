#ifndef COLLISION_H
#define COLLISION_H

#include <QObject>
#include <QTimer>
#include "datas.h"
#include "joueur.h"
#include "missile.h"


class Collision : public QObject
{
        Q_OBJECT
    public:
        explicit Collision(QObject *parent = 0, Datas *datas = 0);
        void demarrer(int ms = 100);
        void stopper();

    signals:
        void log(QString log);
        void logLatence(int latence);
        void collision(Joueur *joueur, Missile *missile);

    public slots:
        void analyserCollision();

    private:
        Datas *datas;
        QTimer *timer;

};

#endif // COLLISION_H
