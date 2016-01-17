#ifndef CONTACT_H
#define CONTACT_H

//#include <qvariant.h>
#include <QDialog>


class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;
class QCheckBox;

class Contact : public QWidget
{
    Q_OBJECT

public:
    Contact( QWidget* parent = 0, Qt::WindowFlags fl = 0 );
    ~Contact();

    QLabel* textLabel1;
    QLabel* textLabel2;
    QLabel* textLabel3;
    QLabel* textLabel4;
    QLabel* textLabel5;
    QLineEdit* lineEditEmail;
    QLineEdit* lineEditAddress;
    QCheckBox* checkBoxMsg;
    QLineEdit* lineEditMobile;
    QLineEdit* lineEditPhone;
    QLineEdit* lineEditZip;

};

#endif // CONTACT_H
