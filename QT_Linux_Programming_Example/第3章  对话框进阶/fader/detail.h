#ifndef DETAIL_H
#define DETAIL_H

#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>

class Detail : public QWidget
{
    Q_OBJECT
public:
    Detail(QWidget *parent = 0, Qt::WindowFlags f1 = 0);
    
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    
    QComboBox *comboBoxCountry;
    QComboBox *comboBoxProvince;
    QLineEdit *lineeditCity;
    QTextEdit *texteditPriv;
};

#endif
