#ifndef LAYOUTDLG_H
#define LAYOUTDLG_H

#include <QtGui>

class LayoutDlg : public QDialog
{
    Q_OBJECT
public:
    LayoutDlg(QWidget *parent = 0, Qt::WindowFlags f = 0);

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
    QPushButton * pushButtonOK;
    QPushButton * pushButtonExit;

};

#endif
