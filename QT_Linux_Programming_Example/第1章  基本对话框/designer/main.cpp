#include <QApplication>
#include <QDialog>
#include "ui_name.h"
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	Ui::Dialog ui;
	QDialog *dialog = new QDialog;
	ui.setupUi(dialog);
	dialog->show();
	return app.exec();
}