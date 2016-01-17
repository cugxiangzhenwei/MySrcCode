#include "pictrans.h"

PicTrans::PicTrans(QWidget *parent)
	: QDialog(parent)
{
  
    QPalette p = palette();
    p.setColor(QPalette::Window,QColor(160,250,200));
    setPalette(p);
    
    QSlider *slider = new QSlider;
    slider->setRange(0,100);
    slider->setTickInterval(10);
    slider->setOrientation(Qt::Horizontal);
    slider->setValue(100);
    
    img = new QImage(":/images/heart.png");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addStretch(1);
    layout->addWidget(slider);
    setLayout(layout);
    
    slotChanged(slider->value());
    
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(slotChanged(int)));
}

void PicTrans::slotChanged(int value)
{
    dst=*img;
    QColor bkColor = palette().color(QPalette::Window);
    float v = 0.01*value;
    int width = img->width();
    int height = img->height();
    for (int h=0;h<height;h++)
    {
    	for (int w=0;w<width;w++)
    	{
    	    int alpha = qAlpha(img->pixel(w,h));
	    if (alpha == 0)	//  ÍêÈ«Í¸Ã÷
	    {
    	    	int red = qRed(bkColor.rgb());
    	    	int green = qGreen(bkColor.rgb());
    	    	int blue = qBlue(bkColor.rgb());
    	    	dst.setPixel(w,h,qRgb(red,green,blue));
	    }
	    else	
	    {
    	    	int red = (int)((qRed(bkColor.rgb()))*(1-v)+(qRed(img->pixel(w,h)))*v);
    	    	int green = (int)((qGreen(bkColor.rgb()))*(1-v)+(qGreen(img->pixel(w,h)))*v);
    	    	int blue = (int)((qBlue(bkColor.rgb()))*(1-v)+(qBlue(img->pixel(w,h)))*v);
    	    	dst.setPixel(w,h,qRgb(red,green,blue));
    	    }
    	}
    }
    
    resize(dst.width(),dst.height());
    repaint();
}

void PicTrans::paintEvent(QPaintEvent * e)
{
    QPainter painter(this);
    painter.drawImage(0,0,dst);
} 

