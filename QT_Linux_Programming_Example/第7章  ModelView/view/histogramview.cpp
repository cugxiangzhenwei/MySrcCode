#include "histogramview.h"

HistogramView::HistogramView(QWidget *parent)
	: QAbstractItemView(parent)
{}

void HistogramView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    QAbstractItemView::dataChanged(topLeft, bottomRight);
    
    viewport()->update();
}

QRect HistogramView::visualRect(const QModelIndex &index) const
{
	return QRect();
}

void HistogramView::scrollTo(const QModelIndex &index, ScrollHint hint)
{}

QModelIndex HistogramView::indexAt(const QPoint &point) const
{
    QPoint newPoint(point.x(),point.y());

    QRegion region;
    foreach(region,listRegionM)	// Male Column
    {   	
    	if (region.contains(newPoint))
    	{
    	    int row = listRegionM.indexOf(region);
    	    QModelIndex index = model()->index(row,1,rootIndex());
    	    return index;
    	}
    }
    
    foreach(region,listRegionF)	// Female Column
    {
    	if (region.contains(newPoint))
    	{
    	    int row = listRegionF.indexOf(region);
    	    QModelIndex index = model()->index(row,2,rootIndex());
    	    return index;
    	}
    }
    
    foreach(region,listRegionS)	// Sum Column
    {
    	if (region.contains(newPoint))
    	{
    	    int row = listRegionS.indexOf(region);
    	    QModelIndex index = model()->index(row,3,rootIndex());
    	    return index;
    	}
    }
    
    return QModelIndex();
}

QModelIndex HistogramView::moveCursor(QAbstractItemView::CursorAction cursorAction,
                            Qt::KeyboardModifiers modifiers)

{
	 return QModelIndex();
}
                           
int HistogramView::horizontalOffset() const
{
	return 0;
}

int HistogramView::verticalOffset() const
{
	return 0;
}

bool HistogramView::isIndexHidden(const QModelIndex &index) const
{
	return false;
}

void HistogramView::setSelectionModel(QItemSelectionModel * selectionModel)
{
    selections = selectionModel;
}

void HistogramView::mousePressEvent(QMouseEvent *e)
{
    QAbstractItemView::mousePressEvent(e);
    setSelection(QRect(e->pos().x(),e->pos().y(),1,1),QItemSelectionModel::SelectCurrent);    
}
    
QRegion HistogramView::itemRegion(QModelIndex index)
{
    QRegion region;
    if (index.column() == 1)		// male
    	region = listRegionM[index.row()];   	
    	
    if (index.column() == 2)		// female
    	region = listRegionF[index.row()];
    	
    if (index.column() == 3)		// retire
    	region = listRegionS[index.row()];

    return region;
}

void HistogramView::setSelection ( const QRect &rect, QItemSelectionModel::SelectionFlags flags )
{
     int rows = model()->rowCount(rootIndex());
     int columns = model()->columnCount(rootIndex());
     QModelIndex selectedIndex;
  
     for (int row = 0; row < rows; ++row) 
     {
         for (int column = 1; column < columns; ++column) 
         {
             QModelIndex index = model()->index(row, column, rootIndex());
             QRegion region = itemRegion(index);
  	     
             if (!region.intersected(rect).isEmpty())
    		 selectedIndex = index;
         }
     }
     
     if(selectedIndex.isValid()) 
         selections->select(selectedIndex,flags);
     else 
     {
         QModelIndex noIndex;
         selections->select(noIndex, flags);
     }
}

QRegion HistogramView::visualRegionForSelection(const QItemSelection &selection) const
{
	return QRegion();
}


void HistogramView::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected )
{
    viewport()->update();
}

void HistogramView::paintEvent(QPaintEvent *)
{
    QPainter painter(viewport());

    painter.setPen(Qt::black);
    int x0 = 40;
    int y0 = 250;
    
    // draw coordinate  
    painter.drawLine(x0, y0, 40, 30);
    painter.drawLine(38, 32, 40, 30);
    painter.drawLine(40, 30, 42, 32);
    painter.drawText(20, 30, tr("num"));
    
    for (int i=1; i<5; i++)
    {
    	painter.drawLine(-1,-i*50,1,-i*50);
    	painter.drawText(-20,-i*50,tr("%1").arg(i*5));
    }

    // xÖá
    painter.drawLine(x0, y0, 540, 250);
    painter.drawLine(538, 248, 540, 250);
    painter.drawLine(540, 250, 538, 252);
    painter.drawText(545, 250, tr("department"));    
    int row;
    // Department
    int posD = x0+20;
    for (row = 0; row < model()->rowCount(rootIndex()); row++) 
    {
    	QModelIndex index = model()->index(row, 0, rootIndex());
    	QString dep = model()->data(index).toString();    
    	
    	painter.drawText(posD,y0+20,dep);
    	posD += 50;
    }
    
    // Male
    int posM = x0+20;
    for (row = 0; row < model()->rowCount(rootIndex()); row++) 
    {
    	QModelIndex index = model()->index(row, 1, rootIndex());
    	int male = model()->data(index).toDouble();
    	
    	int width = 10;
    	
	if (selections->isSelected(index))
	    painter.setBrush(QBrush(Qt::blue,Qt::Dense3Pattern));
      	else
    	    painter.setBrush(Qt::blue);
    	
    	painter.drawRect(QRect(posM,y0-male*10,width,male*10));
    	QRegion regionM(posM,y0-male*10,width,male*10);    	
    	listRegionM << regionM;
    	
    	posM += 50;
    }
    
    // Female
    int posF = x0+30;
    for (row = 0; row < model()->rowCount(rootIndex()); row++) 
    {
    	QModelIndex index = model()->index(row, 2, rootIndex());
    	int female = model()->data(index).toDouble();
    	
    	int width = 10;  	
    	
    	if (selections->isSelected(index))	
   	    painter.setBrush(QBrush(Qt::red,Qt::Dense3Pattern));
    	else
    	    painter.setBrush(Qt::red);
 
    	painter.drawRect(QRect(posF,y0-female*10,width,female*10));
    	QRegion regionF(posF,y0-female*10,width,female*10);    	
    	listRegionF << regionF;
    	
    	posF += 50;
    }
    
    // Retire
    int posS = x0+40;
    for (row = 0; row < model()->rowCount(rootIndex()); row++) 
    {
    	QModelIndex index = model()->index(row, 3, rootIndex());
    	int sum = model()->data(index).toDouble();
    	
    	int width = 10;
    	if (selections->isSelected(index))
    	    painter.setBrush(QBrush(Qt::green,Qt::Dense3Pattern));
    	else
    	    painter.setBrush(QBrush(Qt::green));
    	painter.drawRect(QRect(posS,y0-sum*10,width,sum*10));
   	QRegion regionS(posS,y0-sum*10,width,sum*10);    	
    	listRegionS << regionS;
    	
    	posS += 50;
    }  
}

