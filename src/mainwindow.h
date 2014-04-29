#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "affichage.h"
#include "avion.h"
#include "serveur.h"
#include "client.h"
#include "rechercheserveurs.h"
#include "xml.h"
#include "datas.h"
#include "console.h"
#include "collision.h"
#include "chargement.h"

#include <QMainWindow>
#include <QKeyEvent>
#include <QGraphicsScene>
//Chat
#include <QTextBrowser>
#include <QLineEdit>

#define rad(ang) (M_PI * ang / 180.)
#define deg(ang) (ang / M_PI * 180.)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    signals:
        void log(QString log);

    private slots:
        void on_creerServeur_clicked();
        void on_creerClient_clicked();
        void on_rechercheBouton_clicked();
        void on_suivant_clicked();
        void envoyerRobot();
        void envoyerMessage(QString message);
        void envoyerMessage();
        void envoyerCollision(Joueur *joueurExplose, Missile *missile);
        void serveurDoubleClicked(int row, int column);

        void goPageMenu();
        void goPageChargement();
        void goPageJeu();

        void utiliserIdPerso(QStringList parametres);
        void utiliserIdJoueur(QStringList parametres);
        void utiliserIdRobot(QStringList parametres);
        void utiliserInfoJoueur(QStringList parametres);
        void utiliserInfoServeur(QStringList parametres);
        void utiliserDeconnexion(QStringList parametres);
        void utiliserAction(QStringList parametres);
        void utiliserMessage(QStringList parametres);
        void utiliserTir(QStringList parametres);
        void utiliserCollision(QStringList parametres);
        void utiliserDemandeSynchronisationHorloge(QStringList parametres);
        void utiliserReponseSynchronisationHorloge(QStringList parametres);

        void deconnexionServeur(QString texte);
        void synchroniserHorloge();

    private:
        Ui::MainWindow *ui;

        void keyPressEvent(QKeyEvent* event);
        void keyReleaseEvent(QKeyEvent* event);

        QGraphicsScene *scene;

        Affichage *affichage;
        Client *client;
        Serveur *serveur;
        QTimer *timer;
        Xml *xml;
        Console *console;
        Collision *collision;

        Datas *datas;

        //chat
        QTextBrowser *chatMessages;
        QLineEdit *chatEnvoi;

        //Kill
        QTextBrowser *killMessages;

        //Synchronisation horloge
        QTime delaiSynchronisationHorloge;

        //Chargement
        Chargement *chargement;
};

#endif // MAINWINDOW_H
