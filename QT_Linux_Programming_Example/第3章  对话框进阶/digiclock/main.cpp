#include <QApplication>
#include "digiclock.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    DigiClock clock;
    clock.show();
    return app.exec();
}



