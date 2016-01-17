#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "butterfly.h"

int main(int argc, char * argv[])
{
    QApplication app(argc,argv);

    QGraphicsScene *scene = new QGraphicsScene;
    scene->setSceneRect(QRectF(-200,-200,400,400));
    
    Butterfly *butterfly = new Butterfly;
    butterfly->setPos(-100,0);
    scene->addItem(butterfly);
    
    QGraphicsView *view = new QGraphicsView;
    view->setScene(scene);
    view->setMinimumSize(400,400);
    view->show();
    return app.exec();
}


