#include "console.h"
#include <QVBoxLayout>
#include <QTime>

#include "joueur.h"

Console::Console(QObject *parent, Datas *datas) :
    QObject(parent)
{
    //Affectation
    this->datas = datas;

    //Création des widget
    liste = new QListWidget();
    line = new QLineEdit();

    //Layout des widget
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(liste);
    layout->addWidget(line);

    //Widget contenant le tout
    QWidget *contenuDock = new QWidget;
    contenuDock->setLayout(layout);

    //Création du dock
    dock = new QDockWidget();
    dock->setWidget(contenuDock);

    connect(line, SIGNAL(returnPressed()), this, SLOT(commande()));
    connect(this, SIGNAL(newItem(QListWidgetItem*)), liste, SLOT(scrollToBottom()));

    //Configuration de base
    log_client = false;
    log_serveur = false;
    log_main = false;
    log_collision = false;
    log_collision_latence = false;
    log_avion = false;
    log_horloge = false;

    logCollisionLatenceItem = new QListWidgetItem();

    timerTest = new QTimer(this);
    connect(timerTest, SIGNAL(timeout()), this, SLOT(timerSlot()));
}
//-----------------------------------------------------------------------------------------------------------------------------
int Console::numeroCommande(QString fonction)
{
    if(fonction == "clear")
        return clear;
    else if(fonction == "auteur")
        return auteur;
    else if(fonction == "version")
        return version;
    else if(fonction == "log")
        return log;
    else if(fonction == "joueurs")
        return joueurs;
    else if(fonction == "kick")
    {
        if(datas->moi->admin())
            return kick;
        else
            return pasPermis;
    }
    else if(fonction == "trajectoire")
        return trajectoire;
    else if(fonction == "attaque")
        return attaque;
    else if(fonction == "autopillotage")
        return autopillotage;
    else if(fonction == "test")
        return test;
    else
        return -1;
}
//-----------------------------------------------------------------------------------------------------------------------------
void Console::addItem(QString texte, QColor couleur, bool gras)
{
    QListWidgetItem *item = new QListWidgetItem(texte);
    QFont grasFont;
    grasFont.setBold(gras);
    item->setFont(grasFont);
    item->setTextColor(couleur);
    liste->addItem(item);

    emit newItem(liste->item(liste->count()-1));
}
//-----------------------------------------------------------------------------------------------------------------------------
void Console::addItem(QListWidgetItem *item)
{
    liste->removeItemWidget(item);
    liste->addItem(item);
}
//-----------------------------------------------------------------------------------------------------------------------------
void Console::commande()
{
    QString commande;
    QStringList arguments;
    QString fonction;
    QStringList parametres;
    QString parametre;
    int i;

    commande = line->text();
    line->clear();

    //Pas de commande: stop
    if(commande.isEmpty())
        return;

    //Traitement de la commande
    commande.toLower(); //minuscule
    arguments = commande.split(" ");
    fonction = arguments.at(0);
    parametres = arguments;
    parametres.removeFirst();

    //Ecrit ma commande
    addItem("> " + commande, Qt::blue, true);

    switch(numeroCommande(fonction))
    {
        case clear:
            log_collision_latence = false;
            liste->clear();
            break;

        case auteur:
            addItem("Dany Maillard");
            break;

        case version:
            addItem("dev 0.4x");
            break;

        case log:
            foreach(parametre, parametres)
            {
                if(parametre == "serveur" || parametre == "all")
                {
                    log_serveur = true;
                    addItem("Log du serveur activé");
                }

                if(parametre == "client" || parametre == "all")
                {
                    log_client = true;
                    addItem("Log du client activé");
                }

                if(parametre == "main" || parametre == "all")
                {
                    log_main = true;
                    addItem("Log du jeu activé");
                }

                if(parametre == "collision" || parametre == "all")
                {
                    log_collision = true;
                    log_collision_latence = true;
                    addItem("Log du serveur de collisions activé");
                    addItem(logCollisionLatenceItem);
                }

                if(parametre == "horloge" || parametre == "all")
                {
                    log_horloge = true;
                    addItem("Log des horloges activé");
                }

                if(parametre == "avion" || parametre == "all")
                {
                    log_avion = true;
                    addItem("Log de l'avion activé");
                }

                if(parametre == "stop")
                {
                    log_serveur = false;
                    log_client = false;
                    log_main = false;
                    log_collision = false;
                    log_avion = false;
                    log_horloge = false;

                    liste->removeItemWidget(logCollisionLatenceItem);

                    addItem("Tous les logs sont désactivés");
                }
            }
            break;

        case joueurs:
            foreach (Joueur *i, datas->joueurs)
                addItem(QString("%1 - %2 - %3 (%4 ms)").arg(i->id()).arg(i->pseudo()).arg(i->avion()->score()).arg(i->horloge()->dernierPing()), Qt::black, i->admin());
            break;

        case kick:
            i = parametres.at(0).toInt();
            datas->chercherJoueur(i)->socket()->abort();
            addItem("Joueur kické");
            break;

        case trajectoire:
            i = parametres.at(0).toInt();
            datas->chercherJoueur(i)->avion()->setDessinerTrajectoire(
                        !datas->chercherJoueur(i)->avion()->isDessinerTrajectoire());
            if(datas->chercherJoueur(i)->avion()->isDessinerTrajectoire())
                addItem("Trajectoire activée");
            else
                addItem("Trajectoire désactivée");
            break;

        case attaque:
            datas->moi->avion()->attaqueSpeciale();
            break;

        case autopillotage:
            if(timerTest->isActive())
            {
                timerTest->stop();
                addItem("Auto pillotage désactivé");
            }
            else
            {
                timerTest->start(1200);
                addItem("Auto pillotage activé");
            }
            break;

        case test:
            break;

        case pasPermis:
            addItem("Cette commande n'est utilisable qu'en admin", Qt::red);
            break;

        default:
            addItem("Hein?!", Qt::red);
            break;
    }
}
//-----------------------------------------------------------------------------------------------------------------------------
void Console::logClient(QString log)
{
    if(log_client)
        addItem(QString("[CLIENT] %1: %2").arg(QTime::currentTime().toString()).arg(log));
}
//-----------------------------------------------------------------------------------------------------------------------------
void Console::logServeur(QString log)
{
    if(log_serveur)
        addItem(QString("[SERVEUR] %1: %2").arg(QTime::currentTime().toString()).arg(log));
}
//-----------------------------------------------------------------------------------------------------------------------------
void Console::logMain(QString log)
{
    if(log_main)
    {
        liste->addItem(QString("[MAIN] %1: %2")
                       .arg(QTime::currentTime().toString())
                       .arg(log));
        emit newItem(liste->item(liste->count()-1));
    }
}
//-----------------------------------------------------------------------------------------------------------------------------
void Console::logCollision(QString log)
{
    if(log_collision)
        addItem(QString("[COLLISION] %1: %2").arg(QTime::currentTime().toString()).arg(log));
}
//-----------------------------------------------------------------------------------------------------------------------------
void Console::logCollisionLatence(int latence)
{
    if(log_collision_latence)
    {
        logCollisionLatenceItem->setText(QString("[COLLISION] %1: Latence %2 ms")
                                         .arg(QTime::currentTime().toString())
                                         .arg(latence));
    }
}
//-----------------------------------------------------------------------------------------------------------------------------
void Console::timerSlot()
{
    switch(rand()%6)
    {
        case Avion::accelere: datas->moi->avion()->accelerer(); break;
        case Avion::freine: datas->moi->avion()->freiner(); break;
        case Avion::plane: datas->moi->avion()->planer(); break;
        case Avion::tourneADroite: datas->moi->avion()->droite(); break;
        case Avion::tourneAGauche: datas->moi->avion()->gauche(); break;
        case Avion::tournePlus: datas->moi->avion()->toutDroit(); break;
    }

    datas->moi->avion()->tirer();
}
//-----------------------------------------------------------------------------------------------------------------------------
void Console::logAvion(QString log)
{
    if(log_avion)
        addItem(QString("[AVION] %1: %2").arg(QTime::currentTime().toString()).arg(log));
}
//-----------------------------------------------------------------------------------------------------------------------------
void Console::logHorloge(QString log)
{
    if(log_horloge)
        addItem(QString("[HORLOGE] %1: %2").arg(QTime::currentTime().toString()).arg(log));
}
