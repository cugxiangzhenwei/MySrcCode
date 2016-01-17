#include "spindelegate.h"

SpinDelegate::SpinDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *SpinDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
{
    QSpinBox *editor = new QSpinBox(parent);
    editor->setRange(1000,10000);    

    editor->installEventFilter(const_cast<SpinDelegate*>(this));

    return editor;
}

void SpinDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
    int value = index.model()->data(index).toInt();
    
    QSpinBox *spin = static_cast<QSpinBox*>(editor);
    
    spin->setValue(value);
}

void SpinDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    QSpinBox *spin = static_cast<QSpinBox*>(editor);
    int value = spin->value();

    model->setData(index, value);
}

void SpinDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
