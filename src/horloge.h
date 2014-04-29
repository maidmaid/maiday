#ifndef HORLOGE_H
#define HORLOGE_H

#include <QObject>
#include <QTime>

class Horloge : public QObject
{
        Q_OBJECT
    public:
        explicit Horloge(QObject *parent = 0);
        int id()
        {
            return mId;
        }
        void setId(int id)
        {
            mId = id;
        }
        void init();
        bool synchroniser(int heureDemande, int heureReponse, int heureServeur);
        bool synchroniser(int delaiSynchronisation, int heureServeur);
        double precision();
        int heureServeur();
        int delai()
        {
            return mDelai;
        }
        int nombreSynchronisation()
        {
            return mNombreSynchronisation;
        }
        int dernierPing()
        {
            return mDernierPing;
        }
        void setDernierPing(int dernierPing)
        {
            mDernierPing = dernierPing;
        }

    signals:
        void log(QString log);
        void ping(int ping);

    public slots:

    private:
        int mId;
        int mNombreSynchronisation;
        int mDelai;
        int mHeureDepart;
        QTime temps;
        int mDernierPing;

};

#endif // HORLOGE_H
