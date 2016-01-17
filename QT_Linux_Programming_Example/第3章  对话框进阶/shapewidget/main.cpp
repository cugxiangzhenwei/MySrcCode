#include <QApplication>
#include "shapewidget.h"

int
main(int argc, char * argv[])
{
    QApplication app(argc,argv);
    ShapeWidget shape;
    shape.show();
    return app.exec();
}
