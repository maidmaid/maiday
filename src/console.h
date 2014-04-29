#ifndef CONSOLE_H
#define CONSOLE_H

#include "datas.h"

#include <QObject>
#include <QDockWidget>
#include <QListWidget>
#include <QLineEdit>

class Console : public QObject
{
        Q_OBJECT
    public:
        explicit Console(QObject *parent = 0, Datas *datas = 0);
        QDockWidget *dock;

        enum commandeType
        {
            clear,
            auteur,
            version,
            log,
            joueurs,
            kick,
            pasPermis,
            trajectoire,
            attaque,
            autopillotage,
            test
        };

    signals:
        void newItem(QListWidgetItem *);

    public slots:
        void commande();
        void addItem(QString texte = "", QColor couleur = Qt::black, bool gras = false);
        void addItem(QListWidgetItem *item);
        void logServeur(QString log);
        void logClient(QString log);
        void logMain(QString log);
        void logCollision(QString log);
        void logCollisionLatence(int latence);
        void logAvion(QString log);
        void logHorloge(QString log);
        void timerSlot();

    private:
        Datas *datas;

        QListWidget *liste;
        QLineEdit *line;

        int numeroCommande(QString commande);

        bool log_serveur;
        bool log_client;
        bool log_main;
        bool log_collision;
        bool log_collision_latence;
        bool log_avion;
        bool log_horloge;

        QListWidgetItem *logCollisionLatenceItem;

        QTimer *timerTest;
};

#endif // CONSOLE_H
