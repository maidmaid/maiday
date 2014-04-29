#include "joueur.h"

Joueur::Joueur(QObject *parent) :
    QObject(parent)
{
    mId = 0;
    mAvion = 0;
    mSocket = 0;
    mHorloge = new Horloge(this);
    mPseudo = "";
    mAdmin = false;
    mHote = false;
}
