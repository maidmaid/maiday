#include "chargement.h"

Chargement::Chargement(QWidget *parent) :
    QWidget(parent)
{
    //Widget
    label = new QLabel();
    bar = new QProgressBar();
    bar->setTextVisible(false);

    //Layout
    layout = new QVBoxLayout();
    setMinimumSize(100, 50);
    setMaximumSize(400, 50);
    layout->addWidget(label);
    layout->addWidget(bar);

    setLayout(layout);

    init();
}

void Chargement::init()
{
    label->setText("...");
    bar->setRange(0, 100);
    bar->setValue(0);
}

void Chargement::setValue(QString texte, int pourcentage)
{
    label->setText(texte);
    bar->setValue(pourcentage);
}

void Chargement::setValue(etatChargement etat)
{
    switch(etat)
    {
        case creationServeur:
            setValue("Création du serveur", 20);
        case tentativeConnexion:
            setValue("Tentative de connexion au serveur", 40);
            break;
        case connexion:
            setValue("Connexion réussi: réception des infos du jeu", 60);
            break;
        case synchronisationHorloge:
            setValue("Synchronisation des horloges", 80);
            break;
        case demarrageJeu:
            setValue("Démarrage du jeu en cours...", 100);
            emit chargementTermined();
            break;
    }
}

bool Chargement::isTermined()
{
    bool result;
    result = (bar->value() == bar->maximum()) ? true : false;
    return result;
}
