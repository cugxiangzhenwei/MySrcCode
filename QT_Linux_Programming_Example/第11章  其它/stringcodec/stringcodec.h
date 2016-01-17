#ifndef STRINGCODEC_H
#define STRINGCODEC_H

#include <QtGui>

class StringCodec : public QDialog	
{
    Q_OBJECT
public:
    StringCodec( QWidget *parent=0, Qt::WindowFlags  f=0 );
    ~StringCodec();
public:
    QGridLayout *layout; 
    
    QPushButton *pushButtonStart;

    QLabel *labelSource;
    QLabel *labelUnicode;
    QLabel *labelBigEndianUnicode;
    QLabel *labelUtf8;
    QLabel *labelLocal;

    QLineEdit *lineEditSource;
    QLineEdit *lineEditUnicode; 
    QLineEdit *lineEditBigEndianUnicode;  
    QLineEdit *lineEditUtf8;
    QLineEdit *lineEditLocal;

    QLabel *lenUnicode;   
    QLabel *lenBigEndianUnicode;   
    QLabel *lenUtf8;
    QLabel *lenLocal;

    QLineEdit *lineEditUnicodeLen;   
    QLineEdit *lineEditBigEndianUnicodeLen;
    QLineEdit *lineEditUtf8Len;
    QLineEdit *lineEditLocalLen;
    
private slots:
    void slotStart();
};


#endif 
