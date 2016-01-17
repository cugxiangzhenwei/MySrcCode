#ifndef PROGRESS_H
#define PROGRESS_H

#include <QtGui>

class Progress : public QDialog
{
    Q_OBJECT
public:
    Progress( QWidget *parent=0, Qt::WindowFlags  f=0 );
    ~Progress();
public:
    QLabel *numLabel; 
    QLineEdit *numLineEdit;
    QLabel *typeLabel; 
    QComboBox *typeComboBox;
    
    QProgressBar *progressBar;

    QPushButton *startPushButton;

private slots:
    void slotStart();

};


#endif 
