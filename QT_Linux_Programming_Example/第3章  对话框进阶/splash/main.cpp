#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include "mainwindow.h"

int 
main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QPixmap pixmap(":/splash.jpg");
    QSplashScreen splash(pixmap);
    splash.show();
 //   app.processEvents();

    MainWindow window;
    window.show();
    splash.finish(&window);
    return app.exec();
}
