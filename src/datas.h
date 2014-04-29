#ifndef DATAS_H
#define DATAS_H

#include "joueur.h"
#include "missile.h"
#include "horloge.h"

class Datas : public QObject
{
        Q_OBJECT
    public:
        explicit Datas(QObject *parent = 0);
        QList<Joueur*> joueurs;
        QList<Missile*> missiles;
        Joueur* chercherJoueur(int id);
        Missile* chercherMissile(int id);
        void ajouterJoueur(Joueur *joueur = 0); //mise à joueur d'un joueur
        void supprimerJoueur(Joueur *joueur);
        void supprimerJoueur(int id);
        void supprimerMissile(Missile *missile);
        void supprimerMissile(int id);
        void reset();
        Joueur* moi;

    private slots:
        void init();
    private:

};

#endif // DATAS_H
