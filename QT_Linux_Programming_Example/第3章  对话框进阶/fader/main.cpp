
#include <QApplication>
#include <QTranslator>

#include "configdialog.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(configdialog);

    QApplication app(argc, argv);
    QFont f("AR PL KaitiM GB",12);
    app.setFont(f);
    QTranslator translator;
    translator.load("fader_zh");
    app.installTranslator(&translator);
    ConfigDialog dialog;
    return dialog.exec();
}
