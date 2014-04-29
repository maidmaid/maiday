#include "horloge.h"

Horloge::Horloge(QObject *parent) :
    QObject(parent)
{
    setId(0);
    temps.start();
    init();
}

void Horloge::init()
{
    mDelai = -1;
    mNombreSynchronisation = 0;
    mHeureDepart = 0;
    setDernierPing(-1);
    temps.restart();
}

bool Horloge::synchroniser(int heureDemande, int heureReponse, int heureServeur)
{
    int delaiSynchronisation;

    delaiSynchronisation = heureReponse - heureDemande;

    return synchroniser(delaiSynchronisation, heureServeur);
}

bool Horloge::synchroniser(int delaiSynchronisation, int heureServeur)
{
    bool synchronise;

    setDernierPing(delaiSynchronisation);
    mNombreSynchronisation++;

    synchronise = false;
    if(delaiSynchronisation <= delai() || delai() == -1)
    {
        mHeureDepart = heureServeur + (delaiSynchronisation / 2);
        mDelai = delaiSynchronisation;
        temps.restart();
        synchronise = true;
    }

    emit ping(delaiSynchronisation);
    emit log("Synchronisation n°" + QString::number(nombreSynchronisation() + 1) +
             ": "+ QString::number(heureServeur) +" +- " + QString::number(precision()) + " ms " +
             " (ping à " + QString::number(delaiSynchronisation) + " ms)");

    return synchronise;
}

double Horloge::precision()
{
    return (double)(delai()) / 2.0;
}

int Horloge::heureServeur()
{
    int resultat;

    resultat = -1;
    if(delai() != -1)
        resultat = mHeureDepart + temps.elapsed();

    return resultat;
}
