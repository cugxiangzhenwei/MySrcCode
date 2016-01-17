#ifndef BASEINFO_H
#define BASEINFO_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>

class BaseInfo : public QWidget
{
    Q_OBJECT
public:
    BaseInfo(QWidget *parent = 0, Qt::WindowFlags f1 = 0);

    QLabel * label1;
    QLabel * label2;
    QLabel * label3;
    QLabel * label4;
    QLabel * label5;
    QLabel * label6;
    QLabel * label7;
    QLabel * labelOther;
    QLabel * labelIcon;
    QLineEdit * lineEditUser;
    QLineEdit * lineEditName;
    QComboBox * comboBoxSex;
    QTextEdit * textEditDepartment;
    QLineEdit * lineEditAge;
    QTextEdit * textEditDisc;
    QPushButton * pushButtonIcon;
};

#endif
