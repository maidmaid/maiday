#include "affichage.h"
#include <QDebug>
#include "avion.h"

Affichage::Affichage(QObject *parent, QGraphicsScene *scene, Datas *datas) :
    QObject(parent)
{
    this->scene = scene;
    this->datas = datas;
    view = scene->views().at(0);

    QPixmap fond = QPixmap("img/fond/fond.png");

    scene->setSceneRect(0, 0, fond.width(), fond.height());

    view->setScene(scene);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate); //Update la scène manuellement
    view->setCacheMode(QGraphicsView::CacheBackground); //Cache
    view->setRenderHints(QPainter::TextAntialiasing | QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing); //Méthode d'affichage
    view->setBackgroundBrush(fond);
    view->setGeometry(0, 0, fond.width(), fond.height());
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //Rend invisible la scrollbar H
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //Rend invisible la scrollbar V

    //Decore
    int i;

    //Herbe
    for(i = 0; i < 20; i++)
    {
        herbe = scene->addPixmap(QPixmap("img/fond/herbe.png"));
        herbe->setPos(i * herbe->boundingRect().width(), scene->height() - herbe->boundingRect().height());
    }

    //Arbres
    for(i = 0; i < 5; i++)
    {
        arbre1 = scene->addPixmap(QPixmap("img/fond/arbre1.png"));
        arbre1->setPos(i * 500, scene->height() - arbre1->boundingRect().height());

        arbre2 = scene->addPixmap(QPixmap("img/fond/arbre2.png"));
        arbre2->setPos(30 + i * 300, scene->height() - arbre2->boundingRect().height());
        arbre2->setZValue(2);

        arbre3 = scene->addPixmap(QPixmap("img/fond/arbre3.png"));
        arbre3->setPos(100 + i * 400, scene->height() - arbre3->boundingRect().height());
        if(i%2)
            arbre3->setZValue(1);
    }

    nuage1 = scene->addPixmap(QPixmap("img/fond/nuage1.png"));
    nuage2 = scene->addPixmap(QPixmap("img/fond/nuage2.png"));
    nuage3 = scene->addPixmap(QPixmap("img/fond/nuage3.png"));

    nuage1->setZValue(-1);
    nuage2->setZValue(-1);
    nuage3->setZValue(-1);

    nuage1->scale(0.5, 0.5);
    nuage2->scale(0.5, 0.5);
    nuage3->scale(0.5, 0.5);

    animationNuage1 = new QPropertyAnimation(this, "position1");
    animationNuage2 = new QPropertyAnimation(this, "position2");
    animationNuage3 = new QPropertyAnimation(this, "position3");

    animationNuage1->setDuration(30000);
    animationNuage1->setEasingCurve(QEasingCurve::InOutQuart);
    animationNuage2->setDuration(31000);
    animationNuage2->setEasingCurve(QEasingCurve::OutInQuart);
    animationNuage3->setDuration(32000);
    animationNuage3->setEasingCurve(QEasingCurve::OutInBack);

    animationNuageRoot = new QParallelAnimationGroup(this);
    animationNuageRoot->addAnimation(animationNuage1);
    animationNuageRoot->addAnimation(animationNuage2);
    animationNuageRoot->addAnimation(animationNuage3);

    connect(animationNuageRoot, SIGNAL(finished()), SLOT(update()));

    update();
}

void Affichage::update()
{
    double h;

    h = 100 + rand()%200;

    animationNuage1->setStartValue(QPointF(-nuage1->boundingRect().width(), h));
    animationNuage1->setEndValue(QPointF(scene->width(), h));

    h = h + 100 + rand()%200;

    animationNuage2->setStartValue(QPointF(scene->width(), h));
    animationNuage2->setEndValue(QPointF(-nuage1->boundingRect().width(), h));

    h = h + 100 + rand()%200;

    animationNuage3->setStartValue(QPointF(-nuage3->boundingRect().width(), h));
    animationNuage3->setEndValue(QPointF(scene->width() + 100, h));

    animationNuageRoot->start();
}
