#ifndef PALETTE_H
#define PALETTE_H

#include <QDialog>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QFrame>

class Palette : public QDialog
{
    Q_OBJECT
public:
    Palette(QWidget *parent = 0);

    void createCtrlFrame();
    void createContentFrame();
    
    void fillColorList(QComboBox *);
    
public slots:
    void slotWindow();
    void slotWindowText();
    void slotButton();
    void slotButtonText();
    void slotBase();
    
private:
    QFrame *ctrlFrame;
    QFrame *contentFrame;
    
    QComboBox * windowComboBox;
    QComboBox * windowTextComboBox;
    QComboBox * buttonComboBox;
    QComboBox * buttonTextComboBox;
    QComboBox * baseComboBox;

};

#endif
