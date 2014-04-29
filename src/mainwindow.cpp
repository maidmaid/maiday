#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "avion.h"

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
//chat
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>

#include "missile.h"

//-----------------------------------------------------------------------------------------------------------------------------
//Constructeur
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->creerClient->setVisible(false);
    //ui->edIp->setVisible(false);
    ui->edPortPartie->setVisible(false);
    ui->laPortPartie->setVisible(false);
    ui->laPortServeur->setVisible(false);
    ui->serveurPort->setVisible(false);
    //ui->suivant->setVisible(false);
    ui->menuBar->setVisible(false);
    ui->mainToolBar->setVisible(false);
    ui->statusBar->setVisible(false);

    //Titre
    setWindowTitle("MAIDay");

    //Création de la scene
    scene = new QGraphicsScene(0, 0, 1000, 1000);
    ui->view->setScene(scene);

    //Création de l'array collectant toutes les données des joueurs
    datas = new Datas(this);

    //Création de l'affichage
    affichage = new Affichage(this, scene, datas);

    //Création du client
    client = new Client(this);
    connect(client, SIGNAL(erreurConnexion(QString)), this, SLOT(deconnexionServeur(QString)));
    connect(client, SIGNAL(lireIdPerso(QStringList)), this, SLOT(utiliserIdPerso(QStringList))); //Recu du serveur un id perso
    connect(client, SIGNAL(lireIdJoueur(QStringList)), this, SLOT(utiliserIdJoueur(QStringList))); //Recu du serveur un id d'un joueur
    connect(client, SIGNAL(lireIdRobot(QStringList)), this, SLOT(utiliserIdRobot(QStringList))); //Recu du serveur un id d'un robot
    connect(client, SIGNAL(lireInfoJoueur(QStringList)), this, SLOT(utiliserInfoJoueur(QStringList))); //Recu du serveur les infos d'un joueur
    connect(client, SIGNAL(lireInfoServeur(QStringList)), this, SLOT(utiliserInfoServeur(QStringList))); //Recu les infos serveurs
    connect(client, SIGNAL(lireDeconnexion(QStringList)), this, SLOT(utiliserDeconnexion(QStringList)));
    connect(client, SIGNAL(lireAction(QStringList)), this, SLOT(utiliserAction(QStringList)));
    connect(client, SIGNAL(lireMessage(QStringList)), this, SLOT(utiliserMessage(QStringList)));
    connect(client, SIGNAL(lireTir(QStringList)), this, SLOT(utiliserTir(QStringList)));
    connect(client, SIGNAL(lireCollision(QStringList)), this, SLOT(utiliserCollision(QStringList)));
    connect(client, SIGNAL(lireDemandeSynchronisationHorloge(QStringList)), this, SLOT(utiliserDemandeSynchronisationHorloge(QStringList)));
    connect(client, SIGNAL(lireReponseSynchronisationHorloge(QStringList)), this, SLOT(utiliserReponseSynchronisationHorloge(QStringList)));

    //Création du Xml
    xml = new Xml(this, datas->moi->horloge());

    //Création du serveur
    serveur = new Serveur(this, datas);

    //Moteur pour les collisions
    collision = new Collision(this, datas);
    connect(collision, SIGNAL(collision(Joueur*,Missile*)), SLOT(envoyerCollision(Joueur*,Missile*)));

    //Création de la console
    console = new Console(this, datas);
    addDockWidget(Qt::TopDockWidgetArea, console->dock);
    console->dock->setVisible(false);
    console->dock->setFloating(false);
    connect(client, SIGNAL(erreurConnexion(QString)), console, SLOT(logClient(QString))); //Log problème connexion au serveur
    connect(client, SIGNAL(log(QString)), console, SLOT(logClient(QString))); //Log général du client
    connect(serveur, SIGNAL(log(QString)), console, SLOT(logServeur(QString))); //Log du serveur
    connect(this, SIGNAL(log(QString)), console, SLOT(logMain(QString))); //Log du Main
    connect(collision, SIGNAL(log(QString)), console, SLOT(logCollision(QString))); //Log du moteur de collision
    connect(collision, SIGNAL(logLatence(int)), console, SLOT(logCollisionLatence(int))); //Log du moteur de collision (latence)
    connect(datas->moi->horloge(), SIGNAL(log(QString)), console, SLOT(logHorloge(QString)));

    //Création des widget dans la fenêtre de jeu
    QGridLayout *layout = new QGridLayout();

    //CHAT
    //Création des widget
    chatMessages = new QTextBrowser();
    chatMessages->setStyleSheet("background: transparent; border: transparent;");
    chatMessages->setMaximumHeight(100);
    chatEnvoi = new QLineEdit();
    chatEnvoi->setStyleSheet("background: transparent;");
    connect(chatEnvoi, SIGNAL(returnPressed()), this, SLOT(envoyerMessage()));

    //Layout des widget
    QVBoxLayout *layoutChat = new QVBoxLayout();
    layoutChat->addWidget(chatMessages);
    layoutChat->addWidget(chatEnvoi);

    //Widget contenant le tout
    QWidget *chat = new QWidget();
    chat->setLayout(layoutChat);

    //CARTE
    //Widget contenant le tout
    QLineEdit *lineCarte = new QLineEdit();
    lineCarte->setStyleSheet("background: transparent;");
    lineCarte->setVisible(false);
    QVBoxLayout *layoutCarte = new QVBoxLayout();
    layoutCarte->addWidget(lineCarte);
    QWidget *carte = new QWidget();
    carte->setLayout(layoutCarte);

    //KILL
    killMessages = new QTextBrowser();
    killMessages->setAlignment(Qt::AlignRight);
    killMessages->setStyleSheet("background: transparent; border: transparent;");
    killMessages->setMaximumHeight(100);

    //Mise en forme final
    layout->addWidget(killMessages, 0, 2);
    layout->addWidget(chat, 2, 0);
    layout->addWidget(carte, 2, 2, Qt::AlignBottom);
    layout->setColumnStretch(1, 1);
    layout->setRowStretch(1, 1);

    ui->view->setLayout(layout);

    //Recherche de serveur: IP et Port caché
    ui->rechecheView->hideColumn(1); //Port
    connect(ui->rechecheView, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(serveurDoubleClicked(int,int)));

    //Page de chargement
    chargement = new Chargement();
    ui->pageChargement->layout()->addWidget(chargement);

    //Recherche des serveurs de jeu
    client->recherche();

    //Synchronisation des horloges
    delaiSynchronisationHorloge.start();
}
//-----------------------------------------------------------------------------------------------------------------------------
//Destructeur
MainWindow::~MainWindow()
{
    delete ui;
}
//-----------------------------------------------------------------------------------------------------------------------------
//Gestion de l'appui d'une touche du clavier
void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if(ui->pageJeu->isVisible()) //Si on joue (jeu visible)
    {
        if(!event->isAutoRepeat()) //Au premier appui de la touche, sans les relachements causé par l'autorépétition
        {
            Avion *avion = datas->moi->avion();

            switch(event->key())
            {
                case Qt::Key_A: avion->gauche(); break;
                case Qt::Key_D: avion->droite(); break;
                case Qt::Key_W: avion->accelerer(); break;
                case Qt::Key_S: avion->freiner(); break;
                case Qt::Key_Space: avion->tirer(); break;
            }
        }
    }

    switch(event->key())
    {
        case Qt::Key_section: console->dock->setVisible(!console->dock->isVisible()); break;
        case Qt::Key_Escape: qApp->quit(); break;
    }
}
//-----------------------------------------------------------------------------------------------------------------------------
//Gestion du relachement d'une touche clavier
void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    if(ui->pageJeu->isVisible()) //Si on joue (jeu visible)
    {
        if(!event->isAutoRepeat()) //Au vrai relachement de la touche, sans les relachements causés par l'autorépétition
        {
            Avion *avion = datas->moi->avion();

            switch(event->key())
            {
                case Qt::Key_A: avion->toutDroit(); break;
                case Qt::Key_D: avion->toutDroit(); break;
                case Qt::Key_W: avion->planer(); break;
                case Qt::Key_S: avion->planer(); break;
            }
        }
    }
}
//-----------------------------------------------------------------------------------------------------------------------------
//Slot recevant les infos d'un serveur non-connecté
void MainWindow::utiliserInfoServeur(QStringList parametres)
{
    // parametres[0] == IP
    // parametres[1] == Port
    // parametres[2] == Nom
    // parametres[3] == Total

    //Récupère l'index de la dernière ligne insérée
    int lignes = ui->rechecheView->rowCount();
    ui->rechecheView->insertRow(lignes);

    for (int i = 0; i < 4; ++i)
        ui->rechecheView->setItem(lignes, i, new QTableWidgetItem(parametres.at(i)));
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::serveurDoubleClicked(int row, int column)
{
    QString ip;
    int port;

    ip = ui->rechecheView->item(row, 0)->text();
    port = ui->rechecheView->item(row, 1)->text().toInt();

    client->seConnecter(ip, port);
}
//-----------------------------------------------------------------------------------------------------------------------------
//Slot recevant du serveur connecté un id perso
void MainWindow::utiliserIdPerso(QStringList parametres)
{
    //Affectation des variables
    int id = parametres.at(0).toInt();

    //Page de chargement
    chargement->setValue(Chargement::connexion);

    //Sauvegarde les données
    Joueur *joueur = datas->moi;
    joueur->setId(id);
    joueur->setAvion(new Avion(this, scene, datas, Avion::perso));
    joueur->setPseudo(ui->edPseudo->text());
    datas->ajouterJoueur(joueur);

    connect(joueur->avion(), SIGNAL(infoSortante(QString)), client, SLOT(envoyer(QString)));
    connect(joueur->avion(), SIGNAL(log(QString)), console, SLOT(logAvion(QString)));

    //Démarre l'analyse des colisions si on est hote de la partie
    if(datas->moi->hote())
        collision->demarrer();
    else
        synchroniserHorloge();

    //Envoi au serveur les infos de notre avion
    client->envoyer(xml->ecrireInfoJoueur(datas->moi));

    //Peut démarrer le jeu
    joueur->avion()->demarrer();

    //Page de démarrage
    if(datas->moi->hote())
    {
        chargement->setValue(Chargement::demarrageJeu);
        QTimer::singleShot(500, this, SLOT(goPageJeu()));
    }

    emit log(QString("ID perso: %1").arg(id));
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::utiliserIdJoueur(QStringList parametres)
{
    //Affectation des variables
    int id = parametres.at(0).toInt();

    //Si c'est moi: stop
    if(id == datas->moi->id())
        return;

    //Sauvegarde les données
    Joueur *joueur = datas->chercherJoueur(id);
    joueur->setId(id);
    joueur->setAvion(new Avion(this, scene, datas, Avion::joueur));
    datas->ajouterJoueur(joueur);

    emit log(QString("ID joueur: %1").arg(id));
}
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::utiliserInfoJoueur(QStringList parametres)
{
    //Affectation des variables
    int id = parametres.at(0).toInt();

    //Si c'est moi: stop
    if(id == datas->moi->id())
        return;

    QString pseudo = parametres.at(1);
    bool admin = parametres.at(2).toInt();
    bool hote = parametres.at(3).toInt();
    int vie = parametres.at(4).toInt();
    int score = parametres.at(5).toInt();

    //Sauvegarde les données
    Joueur *joueur = datas->chercherJoueur(id);
    joueur->setPseudo(pseudo);
    joueur->setAdmin(admin);
    joueur->setHote(hote);
    joueur->avion()->setVie(vie);
    joueur->avion()->setScore(score);
    joueur->avion()->demarrer();

    //Information
    chatMessages->append(QString("%1 a rejoint la partie").arg(pseudo));

    emit log(QString("infos joueur: %1").arg(id));
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::utiliserDeconnexion(QStringList parametres)
{
    //Affectation des variables
    int id = parametres.at(0).toInt();

    Joueur *joueur = datas->chercherJoueur(id);

    //Information
    chatMessages->append(QString("%1 a quitté la partie").arg(joueur->pseudo()));

    //Sauvegarde les données
    datas->supprimerJoueur(joueur);

    emit log(QString("Déconnexion: %1").arg(id));
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::utiliserAction(QStringList parametres)
{
    int id = parametres.at(0).toInt();

    //Si c'est moi: stop
    if(datas->moi->id() == id)
        return;

    int temps = parametres.at(1).toInt();
    int action = parametres.at(2).toInt();
    QPointF position = QPointF(parametres.at(3).toDouble(), parametres.at(4).toDouble());
    double angle = parametres.at(5).toDouble();
    double vitesse = parametres.at(6).toDouble();

    //Effectue l'action
    Avion *avion = datas->chercherJoueur(id)->avion();
    //avion->setManuel(position, angle, vitesse);
    //avion->setAction((Avion::actionAvion)(action));

    //!
    avion->repositionner(temps, (Avion::actionAvion)(action), position, angle, vitesse);
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::utiliserTir(QStringList parametres)
{
    int idJoueur = parametres.at(0).toInt();

    //Si c'est moi: stop
    if(idJoueur == datas->moi->id())
        return;

    //! temps ?
    int temps = parametres.at(1).toInt();
    int idMissile = parametres.at(2).toInt();
    QPointF position = QPointF(parametres.at(3).toDouble(), parametres.at(4).toDouble());
    double angle = parametres.at(5).toDouble();
    Missile::typeMissile type = (Missile::typeMissile)parametres.at(6).toInt();

    datas->chercherJoueur(idJoueur)->avion()->tirer(idMissile, position, angle, type);
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::envoyerCollision(Joueur *joueurExplose, Missile *missile)
{
    Joueur *joueurTireur = datas->chercherJoueur(missile->idJoueurTireur);
    client->envoyer(xml->ecrireCollision(joueurTireur, joueurExplose, missile));
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::utiliserCollision(QStringList parametres)
{
    //!
    /*
    if(datas->moi->hote())
        return;
    */

    //! temps ?
    int idTireur = parametres.at(0).toInt();
    int temps = parametres.at(1).toInt();
    int scoreTireur = parametres.at(2).toInt();
    int idExplose = parametres.at(3).toInt();
    int vieExplose = parametres.at(4).toInt();
    int idMissile = parametres.at(5).toInt();

    Joueur *joueurTireur = datas->chercherJoueur(idTireur);
    Joueur *joueurExplose = datas->chercherJoueur(idExplose);
    Missile *missile = datas->chercherMissile(idMissile);

    joueurTireur->avion()->setScore(scoreTireur);
    joueurExplose->avion()->setVie(vieExplose);

    //! EN CAS DE BUG
    if(missile->id == 0)
    {
        missile = new Missile(this, scene, datas, idTireur, idMissile, joueurExplose->avion()->position());
        datas->missiles.append(missile);
        qDebug() << "missile pas reconnu: " << idMissile;
    }

    missile->idJoueurExplose = idExplose;

    //Si joueur a plus que 1 point de vie, il meurt
    if(joueurExplose->avion()->vie() == 1)
    {
        missile->setTypeExplosion(Missile::explosionKill);
        joueurTireur->avion()->ajoutScore(10);
        killMessages->append(QString("%1 killé par %2").arg(joueurTireur->pseudo()).arg(joueurExplose->pseudo()));
    }
    else
    {
        joueurTireur->avion()->ajoutScore(1);
        missile->setTypeExplosion(Missile::explosionCollision);
    }

    missile->exploser();
    joueurExplose->avion()->perdreVie();
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::envoyerMessage(QString message)
{
    client->envoyer(xml->ecrireMessage(datas->moi, message));
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::envoyerMessage()
{
    QString message = chatEnvoi->text();
    chatEnvoi->clear();

    if(!message.isEmpty())
        client->envoyer(xml->ecrireMessage(datas->moi, message));
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::utiliserMessage(QStringList parametres)
{
    // parametres[0] == id
    // parametres[1] == message

    int id;
    QString message;
    QString pseudo;

    id = parametres.at(0).toInt();
    message = parametres.at(1);

    pseudo = datas->chercherJoueur(id)->pseudo();

    chatMessages->append(pseudo + ": " + message);
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::on_creerServeur_clicked()
{
    //Création du serveur
    int port;
    QString nom;

    port = ui->serveurPort->text().toInt();
    nom = ui->edNomServeur->text();

    //Page de chargement
    goPageChargement();
    chargement->setValue(Chargement::creationServeur);

    if(serveur->seConnecter(port))
    {
        serveur->setNom(nom);
        chargement->setValue(Chargement::tentativeConnexion);
        client->seConnecter("127.0.0.1", port);
    }
    else
    {
        QMessageBox::information(this, "Erreur", "Le serveur n'a pas pu démarré", QMessageBox::Ok);
        goPageMenu();
    }
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::on_creerClient_clicked()
{
    //Création du client qui se connecte au serveur
    QString ip = ui->edIp->text();
    int port = ui->edPortPartie->text().toInt();

    //Page de chargement
    goPageChargement();
    chargement->setValue(Chargement::tentativeConnexion);

    client->seConnecter(ip, port);
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::on_rechercheBouton_clicked()
{
    for (int i = 0; i < ui->rechecheView->rowCount(); ++i)
        ui->rechecheView->removeRow(i);

    client->recherche();
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::on_suivant_clicked()
{
    int i = ui->pages->currentIndex() + 1;
    int total = ui->pages->count() - 1;
    ui->pages->setCurrentIndex(i <= total? i : 0);
}
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::envoyerRobot()
{
    if(datas->moi->hote())
    {
        client->envoyer(xml->ecrireIdRobot(1));
    }
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::utiliserIdRobot(QStringList parametres)
{

}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::deconnexionServeur(QString texte)
{
    //Stop le serveur de collision
    if(datas->moi->hote())
        collision->stopper();

    //Réinitialise les datas
    datas->reset();
    chatMessages->clear();

    on_rechercheBouton_clicked();

    if(!texte.isEmpty())
        QMessageBox::information(this, "Connexion serveur", texte, QMessageBox::Ok);

    goPageMenu();
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::synchroniserHorloge()
{
    //Page de chargement
    if(!chargement->isTermined())
        chargement->setValue(Chargement::synchronisationHorloge);

    delaiSynchronisationHorloge.restart();
    client->envoyer(xml->ecrireDemandeSynchronisationHorloge(datas->moi));

    emit log("Demande de synchronisation d'horloge");
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::utiliserDemandeSynchronisationHorloge(QStringList parametres)
{
    int id = parametres.at(0).toInt();
    int dernierPing = parametres.at(1).toInt();

    //Inscrit le dernier ping du demandeur
    datas->chercherJoueur(id)->horloge()->setDernierPing(dernierPing);

    //Seul le serveur peut envoyer son horloge au demandeur
    if(!datas->moi->hote())
        return;

    //Renvoie l'heure du serveur au demandeur
    serveur->envoyer(id, xml->ecrireReponseSynchronisationHorloge());

    emit log("Reponse de synchronisation d'horloge");
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::utiliserReponseSynchronisationHorloge(QStringList parametres)
{
    int delai = delaiSynchronisationHorloge.elapsed();
    int heureServeur = parametres.at(0).toInt();

    //Synchronise l'horloge du client
    datas->moi->horloge()->synchroniser(delai, heureServeur);

    //Effectue 20 synchronisation au début
    if(datas->moi->horloge()->nombreSynchronisation() < 20)
        synchroniserHorloge();
    else
    {
        //Page de chargement. Effectue une synchronisation toutes les 5 secondes
        if(!chargement->isTermined())
        {
            chargement->setValue(Chargement::demarrageJeu);
            QTimer::singleShot(500, this, SLOT(goPageJeu()));
        }

        QTimer::singleShot(5 * 1000, this, SLOT(synchroniserHorloge()));
    }

}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::goPageMenu()
{
    ui->pages->setCurrentWidget(ui->pageMenu);
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::goPageChargement()
{
    ui->pages->setCurrentWidget(ui->pageChargement);
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void MainWindow::goPageJeu()
{
    ui->pages->setCurrentWidget(ui->pageJeu);
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
