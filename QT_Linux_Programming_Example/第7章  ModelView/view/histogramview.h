#ifndef HISTOGRAMVIEW_H
#define HISTOGRAMVIEW_H

#include <QtGui>

class HistogramView : public QAbstractItemView
{
    Q_OBJECT
public:
    HistogramView(QWidget *parent=0);
    
    QRect visualRect(const QModelIndex &index)const;
    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
    QModelIndex indexAt(const QPoint &point) const;    
    
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

    void setSelectionModel(QItemSelectionModel * selectionModel);
    QRegion itemRegion(QModelIndex index);  
    
protected slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected );
    
protected:
    QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction,
                            Qt::KeyboardModifiers modifiers);
    int horizontalOffset() const;
    int verticalOffset() const;    
    bool isIndexHidden(const QModelIndex &index) const;
    void setSelection ( const QRect&rect, QItemSelectionModel::SelectionFlags flags );
    QRegion visualRegionForSelection(const QItemSelection &selection) const;       
    
private:
    QItemSelectionModel *selections; 
    
    QList<QRegion> listRegionM;  
    QList<QRegion> listRegionF; 
    QList<QRegion> listRegionS; 
    
};

#endif 
