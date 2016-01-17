#include <QApplication>

#include "settings.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    Settings *s = new Settings();
    s->show();
    return app.exec();
}
