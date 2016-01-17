#include <QApplication>
#include "pictrans.h"

int
main(int argc, char * argv[])
{
    QApplication app(argc,argv);
    PicTrans pic;
    pic.show();
    return app.exec();
}
