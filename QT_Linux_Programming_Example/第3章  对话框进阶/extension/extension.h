#ifndef EXTENSION_H
#define EXTENSION_H

#include <QtGui>

class Extension : public QDialog
{
    Q_OBJECT
public:
    Extension(QWidget *parent=0);
    
    void createBaseInfo();
    void createDetailInfo();

public slots:
    void slotExtension();
    
private:
    QWidget *baseWidget;
    QWidget *detailWidget;
};

#endif 
