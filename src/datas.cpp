#include "datas.h"
#include "joueur.h"

//-----------------------------------------------------------------------------------------------------------------------------
Datas::Datas(QObject *parent) :
    QObject(parent)
{
    init();
}
//-----------------------------------------------------------------------------------------------------------------------------
void Datas::init()
{
    moi = new Joueur(this);
    joueurs.append(moi);
}
//-----------------------------------------------------------------------------------------------------------------------------
Joueur* Datas::chercherJoueur(int id)
{
    foreach (Joueur *i, joueurs)
    {
        if(i->id() == id)
            return i;
    }

    return new Joueur(this);
}
//-----------------------------------------------------------------------------------------------------------------------------
Missile* Datas::chercherMissile(int id)
{
    foreach (Missile *i, missiles)
    {
        if(i->id == id)
            return i;
    }

    return new Missile(this);
}
//-----------------------------------------------------------------------------------------------------------------------------
void Datas::ajouterJoueur(Joueur *joueur)
{
    Joueur *joueurRecherche = chercherJoueur(joueur->id());

    //Joueur n'existe pas: ajout
    if(joueurRecherche->id() == 0)
    {
        joueurs.append(joueur);
    }
    else
    {
        //Joueur existe: mise à jour
        if(joueur->avion() != joueurRecherche->avion() && joueur->avion() != 0)
            joueurRecherche->setAvion(joueur->avion());

        if(joueur->pseudo() != joueurRecherche->pseudo() && !joueur->pseudo().isEmpty())
            joueurRecherche->setPseudo(joueur->pseudo());

        if(joueur->socket() != joueurRecherche->socket() && joueur->socket() != 0)
            joueurRecherche->setSocket(joueur->socket());

        joueurRecherche->setAdmin(joueur->admin());
        joueurRecherche->setHote(joueur->hote());
    }
}
//-----------------------------------------------------------------------------------------------------------------------------
void Datas::supprimerJoueur(Joueur *joueur)
{
    joueur->avion()->deleteLater();
    joueur->socket()->deleteLater();
    joueurs.removeOne(joueur);

    //Supprime tous ses missiles de la collection
    foreach (Missile *i, missiles)
    {
        if(i->idJoueurTireur == joueur->id())
            missiles.removeOne(i);
    }
}
//-----------------------------------------------------------------------------------------------------------------------------
void Datas::supprimerJoueur(int id)
{
    Joueur *joueur = chercherJoueur(id);
    supprimerJoueur(joueur);
}
//-----------------------------------------------------------------------------------------------------------------------------
void Datas::reset()
{
    foreach (Joueur *joueur, joueurs)
        supprimerJoueur(joueur);
    foreach (Missile *missile, missiles)
        supprimerMissile(missile);

    init();
}
//-----------------------------------------------------------------------------------------------------------------------------
void Datas::supprimerMissile(Missile *missile)
{
    missiles.removeOne(missile);
}
//-----------------------------------------------------------------------------------------------------------------------------
void Datas::supprimerMissile(int id)
{
    Missile *missile = chercherMissile(id);
    supprimerMissile(missile);
}
