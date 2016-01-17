#ifndef STYLE_H
#define STYLE_H

#include <QDialog>

class QFrame;

class Style : public QDialog
{
    Q_OBJECT
public:
    Style(QWidget *parent=0);
    
    void createLeftLayout();
    void createRightLayout();
    
public slots:
    void slotChangeStyle(QString);
    
private:
    QFrame *leftFrame;
    QFrame *rightFrame;
};

#endif  // STYLE_H
