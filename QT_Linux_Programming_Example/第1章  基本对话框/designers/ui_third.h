/********************************************************************************
** Form generated from reading ui file 'third.ui'
**
** Created: Mon May 12 17:36:10 2008
**      by: Qt User Interface Compiler version 4.3.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_THIRD_H
#define UI_THIRD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

class Ui_Third
{
public:
    QLabel *label;
    QPushButton *closePushButton;

    void setupUi(QWidget *Third)
    {
    if (Third->objectName().isEmpty())
        Third->setObjectName(QString::fromUtf8("Third"));
    QSize size(300, 300);
    size = size.expandedTo(Third->minimumSizeHint());
    Third->resize(size);
    label = new QLabel(Third);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(80, 70, 131, 16));
    closePushButton = new QPushButton(Third);
    closePushButton->setObjectName(QString::fromUtf8("closePushButton"));
    closePushButton->setGeometry(QRect(180, 230, 75, 23));

    retranslateUi(Third);
    QObject::connect(closePushButton, SIGNAL(clicked()), Third, SLOT(close()));

    QMetaObject::connectSlotsByName(Third);
    } // setupUi

    void retranslateUi(QWidget *Third)
    {
    Third->setWindowTitle(QApplication::translate("Third", "Form", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Third", "\350\277\231\346\230\257\347\254\254\344\270\211\344\270\252 UI !!", 0, QApplication::UnicodeUTF8));
    closePushButton->setText(QApplication::translate("Third", "\345\205\263\351\227\255", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Third);
    } // retranslateUi

};

namespace Ui {
    class Third: public Ui_Third {};
} // namespace Ui

#endif // UI_THIRD_H
