#ifndef PICTRANS_H
#define PICTRANS_H

#include <QtGui>

class QImage;
class QPixmap;

class PicTrans : public QDialog
{
    Q_OBJECT
public:
    PicTrans(QWidget *parent=0);
    
    void paintEvent(QPaintEvent *);

public slots:
    void slotChanged(int);
    
private:
    QImage *img;
    QImage dst;
};
#endif 
