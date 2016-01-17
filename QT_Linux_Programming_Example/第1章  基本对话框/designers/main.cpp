#include "mywidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QFont font("ZYSong18030",12);
    app.setFont(font);	

    MyWidget *w = new MyWidget;
    w->show();
    return app.exec();
}