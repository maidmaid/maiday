#ifndef CHARGEMENT_H
#define CHARGEMENT_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>

class Chargement : public QWidget
{
        Q_OBJECT
    public:
        explicit Chargement(QWidget *parent = 0);
        enum etatChargement
        {
            creationServeur,
            tentativeConnexion,
            connexion,
            receptionInfo,
            synchronisationHorloge,
            demarrageJeu
        };

    signals:
        void chargementTermined();

    public slots:
        bool isTermined();
        void init();
        void setValue(QString texte, int pourcentage);
        void setValue(etatChargement etat);

    private:
        QWidget *widget;
        QVBoxLayout *layout;
        QLabel *label;
        QProgressBar *bar;
};

#endif // CHARGEMENT_H
