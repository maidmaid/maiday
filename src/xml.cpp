#include "xml.h"
#include "joueur.h"

#include <QXmlStreamReader>
#include <QDebug>


Xml::Xml(QObject *parent, Horloge *horloge) :
    QObject(parent)
{
    xml = new QXmlStreamWriter(&texte);
    xml->setAutoFormatting(true);
    mHorloge = horloge;
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void Xml::lire(QString source)
{
    source = "<root>" + source + "</root>";
    QXmlStreamReader texte(source);
    QStringList parametres;

    while (!texte.atEnd())
    {
        texte.readNext();
        parametres.clear();

        if (!texte.isStartElement())
            continue;

        if(texte.name() == "idPerso")
        {
            parametres << texte.readElementText();
            emit lireIdPerso(parametres);
        }

        else if(texte.name() == "idJoueur")
        {
            parametres << texte.readElementText();
            emit lireIdJoueur(parametres);
        }

        else if(texte.name() == "idRobot")
        {
            parametres << texte.readElementText();
            emit lireIdRobot(parametres);
        }

        else if(texte.name() == "infoJoueur")
        {
            while(!(texte.name() == "infoJoueur" && texte.isEndElement()))
            {
                texte.readNext();

                if(texte.name() == "id" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "pseudo" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "admin" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "hote" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "vie" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "score" && texte.isStartElement())
                    parametres << texte.readElementText();
            }

            emit lireInfoJoueur(parametres);
        }

        else if(texte.name() == "infoServeur")
        {

            while(!(texte.name() == "infoServeur" && texte.isEndElement()))
            {
                texte.readNext();

                if(texte.name() == "ip" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "port" && texte.isStartElement())
                    parametres << "34340";
                else if(texte.name() == "nom" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "total" && texte.isStartElement())
                    parametres << texte.readElementText();

            }

            emit lireInfoServeur(parametres);
        }

        else if(texte.name() == "deconnexion")
        {
            parametres << texte.readElementText();
            emit lireDeconnexion(parametres);
        }

        else if(texte.name() == "action")
        {
            while(!(texte.name() == "action" && texte.isEndElement()))
            {
                texte.readNext();

                if(texte.name() == "id" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "temps" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "actionAvion" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "x" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "y" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "angle" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "vitesse" && texte.isStartElement())
                    parametres << texte.readElementText();
            }

            emit lireAction(parametres);
        }

        else if(texte.name() == "message")
        {
            while(!(texte.name() == "message" && texte.isEndElement()))
            {
                texte.readNext();

                if(texte.name() == "id" && texte.isStartElement())
                    parametres << texte.readElementText();
                if(texte.name() == "message" && texte.isStartElement())
                    parametres << texte.readElementText();
            }

            emit lireMessage(parametres);
        }

        else if(texte.name() == "tir")
        {
            while(!(texte.name() == "tir" && texte.isEndElement()))
            {
                texte.readNext();

                if(texte.name() == "idJoueur" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "temps" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "id" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "x" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "y" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "angle" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "type" && texte.isStartElement())
                    parametres << texte.readElementText();
            }

            emit lireTir(parametres);
        }

        else if(texte.name() == "collision")
        {
            while(!(texte.name() == "collision" && texte.isEndElement()))
            {
                texte.readNext();

                if(texte.name() == "idTireur" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "temps" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "scoreTireur" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "idExplose" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "vieExplose" && texte.isStartElement())
                    parametres << texte.readElementText();
                else if(texte.name() == "idMissile" && texte.isStartElement())
                    parametres << texte.readElementText();
            }

            emit lireCollision(parametres);
        }

        else if(texte.name() == "demandeSynchronisationHorloge")
        {
            while(!(texte.name() == "demandeSynchronisationHorloge" && texte.isEndElement()))
            {
                texte.readNext();

                if(texte.name() == "id" && texte.isStartElement())
                    parametres << texte.readElementText();
                if(texte.name() == "dernierPing" && texte.isStartElement())
                    parametres << texte.readElementText();
            }

            emit lireDemandeSynchronisationHorloge(parametres);
        }

        else if(texte.name() == "reponseSynchronisationHorloge")
        {
            parametres << texte.readElementText();
            emit lireReponseSynchronisationHorloge(parametres);
        }
    }
}

//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireIdPerso(int id)
{
    texte.clear();

    xml->writeTextElement("idPerso", QString::number(id));

    return texte.toUtf8();
}
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireIdPerso(Joueur *joueur)
{
    return ecrireIdPerso(joueur->id());
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireIdJoueur(int id)
{
    texte.clear();

    xml->writeTextElement("idJoueur", QString::number(id));

    return texte.toUtf8();
}
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireIdJoueur(Joueur *joueur)
{
    return ecrireIdJoueur(joueur->id());
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireInfoJoueur(int id, QString pseudo, bool admin, bool hote, int vie, int score)
{
    texte.clear();

    xml->writeStartElement("infoJoueur");
    xml->writeTextElement("id", QString::number(id));
    xml->writeTextElement("pseudo", pseudo);
    xml->writeTextElement("admin", QString::number(admin));
    xml->writeTextElement("hote", QString::number(hote));
    xml->writeTextElement("vie", QString::number(vie));
    xml->writeTextElement("score", QString::number(score));
    xml->writeEndElement(); // </infoJoueur>

    return texte.toUtf8();
}
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireInfoJoueur(Joueur *joueur)
{
    return ecrireInfoJoueur(joueur->id(), joueur->pseudo(), joueur->admin(),
                            joueur->hote(), joueur->avion()->vie(), joueur->avion()->score());
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireIdRobot(int id)
{
    texte.clear();

    xml->writeTextElement("idRobot", QString::number(id));

    return texte.toUtf8();
}
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireIdRobot(Joueur *joueur)
{
    return ecrireIdRobot(joueur->id());
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireInfoServeur(QString ip, int port, QString nom, int total)
{
    texte.clear();

    xml->writeStartElement("infoServeur");
    xml->writeTextElement("ip", ip);
    xml->writeTextElement("port", QString::number(port));
    xml->writeTextElement("nom", nom);
    xml->writeTextElement("total", QString::number(total));
    xml->writeEndElement(); // </infoServeur>

    return texte.toUtf8();
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireDeconnexion(int id)
{
    texte.clear();

    xml->writeTextElement("deconnexion", QString::number(id));

    return texte.toUtf8();
}
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireDeconnexion(Joueur *joueur)
{
    return ecrireDeconnexion(joueur->id());
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireAction(int id, int action, QPointF pos, double angle, double vitesse)
{
    texte.clear();

    xml->writeStartElement("action");
    xml->writeTextElement("id", QString::number(id));
    xml->writeTextElement("temps", temps());
    xml->writeTextElement("actionAvion", QString::number(action));
    xml->writeTextElement("x", QString::number(pos.x()));
    xml->writeTextElement("y", QString::number(pos.y()));
    xml->writeTextElement("angle", QString::number(angle));
    xml->writeTextElement("vitesse", QString::number(vitesse));
    xml->writeEndElement(); // </action>

    return texte.toUtf8();
}
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireAction(Joueur *joueur)
{
    Avion *avion = joueur->avion();
    return ecrireAction(joueur->id(), avion->action(), avion->position(), avion->angle(), avion->vitesse());
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireMessage(int id, QString message)
{
    texte.clear();

    xml->writeStartElement("message");
    xml->writeTextElement("id", QString::number(id));
    xml->writeTextElement("message", message);
    xml->writeEndElement(); // </message>

    return texte.toUtf8();
}
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireMessage(Joueur *joueur, QString message)
{
    return ecrireMessage(joueur->id(), message);
}
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireTir(int idJoueur, int id, QPointF position, double angle, int type)
{
    texte.clear();

    xml->writeStartElement("tir");
    xml->writeTextElement("idJoueur", QString::number(idJoueur));
    xml->writeTextElement("temps", temps());
    xml->writeTextElement("id", QString::number(id));
    xml->writeTextElement("x", QString::number(position.x()));
    xml->writeTextElement("y", QString::number(position.y()));
    xml->writeTextElement("angle", QString::number(angle));
    xml->writeTextElement("type", QString::number(type));
    xml->writeEndElement(); // </tir>

    return texte.toUtf8();
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireTir(Missile *missile)
{
    return ecrireTir(missile->idJoueurTireur, missile->id, missile->position(), missile->angle(), missile->typeDeMissile());
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireCollision(int idTireur, int scoreTireur, int idExplose, int vieExplose, int idMissile)
{
    texte.clear();

    xml->writeStartElement("collision");
    xml->writeTextElement("idTireur", QString::number(idTireur));
    xml->writeTextElement("temps", temps());
    xml->writeTextElement("scoreTireur", QString::number(scoreTireur));
    xml->writeTextElement("idExplose", QString::number(idExplose));
    xml->writeTextElement("vieExplose", QString::number(vieExplose));
    xml->writeTextElement("idMissile", QString::number(idMissile));
    xml->writeEndElement(); // </collision>

    return texte.toUtf8();
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireCollision(Joueur *joueurTireur, Joueur *joueurExplose, Missile *missile)
{
    return ecrireCollision(joueurTireur->id(), joueurTireur->avion()->score(),
                           joueurExplose->id(), joueurExplose->avion()->vie(),
                           missile->id);
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireDemandeSynchronisationHorloge(int id, int dernierPing)
{
    texte.clear();

    xml->writeStartElement("demandeSynchronisationHorloge");
    xml->writeTextElement("id", QString::number(id));
    xml->writeTextElement("dernierPing", QString::number(dernierPing));
    xml->writeEndElement(); // </demandeSynchronisationHorloge>

    return texte.toUtf8();
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireDemandeSynchronisationHorloge(Joueur *joueur)
{
    return ecrireDemandeSynchronisationHorloge(joueur->id(), joueur->horloge()->dernierPing());
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
QByteArray Xml::ecrireReponseSynchronisationHorloge()
{
    texte.clear();

    xml->writeTextElement("reponseSynchronisationHorloge", temps());

    return texte.toUtf8();
}
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
