/********************************************************************************
** Form generated from reading ui file 'first.ui'
**
** Created: Mon May 12 17:37:29 2008
**      by: Qt User Interface Compiler version 4.3.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FIRST_H
#define UI_FIRST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

class Ui_First
{
public:
    QLabel *label;
    QPushButton *childPushButton;

    void setupUi(QWidget *First)
    {
    if (First->objectName().isEmpty())
        First->setObjectName(QString::fromUtf8("First"));
    QSize size(300, 300);
    size = size.expandedTo(First->minimumSizeHint());
    First->resize(size);
    label = new QLabel(First);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(90, 80, 131, 16));
    childPushButton = new QPushButton(First);
    childPushButton->setObjectName(QString::fromUtf8("childPushButton"));
    childPushButton->setGeometry(QRect(190, 240, 75, 23));

    retranslateUi(First);

    QMetaObject::connectSlotsByName(First);
    } // setupUi

    void retranslateUi(QWidget *First)
    {
    First->setWindowTitle(QApplication::translate("First", "Form", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("First", "\350\277\231\346\230\257\347\254\254\344\270\200\344\270\252 UI !!", 0, QApplication::UnicodeUTF8));
    childPushButton->setText(QApplication::translate("First", "\345\255\220\347\252\227\345\217\243", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(First);
    } // retranslateUi

};

namespace Ui {
    class First: public Ui_First {};
} // namespace Ui

#endif // UI_FIRST_H
