#ifndef INPUTDLG_H
#define INPUTDLG_H

#include <QtGui>

class InputDlg : public QDialog	
{
    Q_OBJECT
public:
    InputDlg();

public:
    QPushButton *nameButton;
    QPushButton *sexButton;
    QPushButton *ageButton;
    QPushButton *statureButton;
    
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    QLabel *nameLabel;
    QLabel *sexLabel;
    QLabel *ageLabel;
    QLabel *statureLabel;

private slots:
    void slotName();
    void slotSex();
    void slotAge();
    void slotStature();
 
};


#endif

