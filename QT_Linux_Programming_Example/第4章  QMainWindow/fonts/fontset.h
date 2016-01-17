#ifndef FONTSET_H
#define FONTSET_H

#include <QtGui>

class FontSet : public QMainWindow
{
    Q_OBJECT
public:
    FontSet(QWidget *parent=0);
    
    QLabel *label1;
    QLabel *label2;
    
    QFontComboBox *fontBox;
    QComboBox *sizeBox;
    QToolButton *boldBtn;
    QToolButton *italicBtn;
    QToolButton *underBtn;
    QToolButton *colorBtn;
    
    void mergeFormat(QTextCharFormat);
    
public slots:
    void slotFont(QString);
    void slotSize(QString);
    void slotBold();
    void slotItalic();
    void slotUnder();
    void slotColor();
    void slotCurrentFormatChanged(const QTextCharFormat &fmt);
    
private:
    QTextEdit *text;
};

#endif 
