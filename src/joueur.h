#ifndef JOUEUR_H
#define JOUEUR_H

#include "avion.h"
#include "horloge.h"

#include <QObject>
#include <QHostAddress>
#include <QPointF>
#include <QTcpSocket>

class Joueur : public QObject
{
        Q_OBJECT
    public:
        explicit Joueur(QObject *parent = 0);

        int id()
        {
            return mId;
        }
        void setId(int id)
        {
            mId = id;

            if(mAvion != 0)
                mAvion->id = id;

            if(mHorloge != 0)
                mHorloge->setId(id);
        }

        Avion * avion()
        {
            return mAvion;
        }
        void setAvion(Avion *avion)
        {
            mAvion = avion;
            mAvion->id = mId;
        }

        QTcpSocket * socket()
        {
            return mSocket;
        }
        void setSocket(QTcpSocket *socket)
        {
            mSocket = socket;
        }

        QString pseudo()
        {
            return mPseudo;
        }
        void setPseudo(QString pseudo)
        {
            pseudo = pseudo.left(15);

            if(!pseudo.isEmpty() && mPseudo != pseudo)
            {
                mPseudo = pseudo;
                mAvion->setPseudo(pseudo);
            }
        }

        bool admin()
        {
            return mAdmin;
        }
        void setAdmin(bool admin)
        {
            mAdmin = admin;
        }

        bool hote()
        {
            return mHote;
        }
        void setHote(bool hote)
        {
            mHote = hote;
        }
        void setHorloge(Horloge* horloge)
        {
            mHorloge = horloge;
        }
        Horloge* horloge()
        {
            return mHorloge;
        }


    signals:

    public slots:

    private:
        int mId;
        QTcpSocket *mSocket;
        QString mPseudo;
        Avion *mAvion;
        Horloge *mHorloge;
        bool mAdmin;
        bool mHote;

};

#endif // JOUEUR_H
