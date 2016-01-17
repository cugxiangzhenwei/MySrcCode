#include "combodelegate.h"

ComboDelegate::ComboDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *ComboDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->addItem(QString::fromLocal8Bit("工人"));
    editor->addItem(QString::fromLocal8Bit("农民"));
    editor->addItem(QString::fromLocal8Bit("医生"));
    editor->addItem(QString::fromLocal8Bit("律师"));
    editor->addItem(QString::fromLocal8Bit("军人"));

    editor->installEventFilter(const_cast<ComboDelegate*>(this));

    return editor;
}

void ComboDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
    QString str = index.model()->data(index).toString();
    
    QComboBox *box = static_cast<QComboBox*>(editor);
    int i = box->findText(str);
    box->setCurrentIndex(i);
}

void ComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    QComboBox *box = static_cast<QComboBox*>(editor);
    QString str = box->currentText();

    model->setData(index, str);
}

void ComboDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

