#ifndef CREATSHIP_H
#define CREATSHIP_H
#include <QDialog>
#include "ui_Creatship.h"
class CreatShip:public QDialog,public Ui_CreatShip
{
	Q_OBJECT
public:
	CreatShip(QWidget *parent£½0);
private slots:
void OpenRaster();
void SaveShape();
void EnableOkBtn(const QString &);
void CreateBtnclicked();
};
#endif