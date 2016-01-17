#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QFont f("ZYSong18030",10);
    QApplication::setFont(f);
    QApplication app(argc,argv);
    MainWindow *mainwindow = new MainWindow();
    mainwindow->show();
    return app.exec();
}
