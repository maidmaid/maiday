#include "collision.h"
#include "avion.h"
#include "joueur.h"

Collision::Collision(QObject *parent, Datas *datas) :
    QObject(parent)
{
    this->datas = datas;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(analyserCollision()));

    emit log("Création d'un serveur de collision inactif");
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Collision::analyserCollision()
{
    QTime temps;
    temps.start();

    //Passe chaque joueur en revu
    foreach (Joueur *joueur, datas->joueurs)
    {
        //Passe chaque missile en revu
        foreach (Missile *missile, datas->missiles)
        {
            //Pas ses propres missiles + Pas déjà en collision
            if(missile->idJoueurTireur == joueur->id() || missile->isCollision())
                continue;

            //Missile en collision avec l'avion du joueur?
            if(missile->collidesWithItem(joueur->avion()))
            {
                missile->setCollision(true);
                emit collision(joueur, missile);
            }
        }
    }

    emit logLatence(temps.elapsed());
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Collision::demarrer(int ms)
{
    timer->start(ms);
    emit log("Le serveur de collision a démarré");
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Collision::stopper()
{
    timer->stop();
    emit log("Le serveur de collision a été stoppé");
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------

