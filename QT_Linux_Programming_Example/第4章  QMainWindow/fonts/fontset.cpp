#include "fontset.h"
#include <QtGui>

FontSet::FontSet(QWidget *parent)
	: QMainWindow(parent)
{
    QFont f("ZYSong18030",12);
    setFont(f);
    
    setWindowTitle(tr("font"));
  
    QToolBar *toolBar = addToolBar("Font");
    
    QLabel *label1 = new QLabel(tr("ZiTi: "));
    fontBox = new QFontComboBox(toolBar);
    fontBox->setFontFilters(QFontComboBox::ScalableFonts);
    toolBar->addWidget(label1);
    toolBar->addWidget(fontBox);

    QLabel *label2 = new QLabel(tr("number: "));
    sizeBox = new QComboBox(toolBar);
    toolBar->addWidget(label2);
    toolBar->addWidget(sizeBox);
    QFontDatabase db;
    foreach(int size, db.standardSizes())
    {
    	sizeBox->addItem(QString::number(size));
    }
    
    toolBar->addSeparator();
    
    boldBtn = new QToolButton;
    boldBtn->setIcon(QIcon(":/images/bold.png"));
    boldBtn->setCheckable(true);
    toolBar->addWidget(boldBtn);
    italicBtn = new QToolButton;
    italicBtn->setIcon(QIcon(":/images/italic.png"));
    italicBtn->setCheckable(true);
    toolBar->addWidget(italicBtn);
 
    underBtn = new QToolButton;
    underBtn->setIcon(QIcon(":/images/underline.png"));
    underBtn->setCheckable(true);
    toolBar->addWidget(underBtn);

    
    toolBar->addSeparator();
    colorBtn = new QToolButton;
    colorBtn->setIcon(QIcon(":/images/color.png"));
    toolBar->addWidget(colorBtn);
        
    text =  new QTextEdit(this);
    text->setFocus();
    setCentralWidget(text);
    
    connect(fontBox,SIGNAL(activated(QString)),this,SLOT(slotFont(QString)));
    connect(sizeBox,SIGNAL(activated(QString)),this,SLOT(slotSize(QString)));
    connect(boldBtn,SIGNAL(clicked()),this,SLOT(slotBold()));
    connect(italicBtn,SIGNAL(clicked()),this,SLOT(slotItalic()));
    connect(underBtn,SIGNAL(clicked()),this,SLOT(slotUnder()));
    connect(colorBtn,SIGNAL(clicked()),this,SLOT(slotColor()));
    
    connect(text,SIGNAL(currentCharFormatChanged(const QTextCharFormat&)),this,SLOT(slotCurrentFormatChanged(const QTextCharFormat&)));

}

void FontSet::slotFont(QString f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);    
    mergeFormat(fmt);
}

void FontSet::slotSize(QString num)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(num.toFloat());    
    mergeFormat(fmt);
}

void FontSet::slotBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(boldBtn->isChecked()? QFont::Bold : QFont::Normal);    
    mergeFormat(fmt);
}

void FontSet::slotItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(italicBtn->isChecked());    
    mergeFormat(fmt);
}

void FontSet::slotUnder()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(underBtn->isChecked());    
    mergeFormat(fmt);
}

void FontSet::slotColor()
{
    QColor color = QColorDialog::getColor (Qt::red,this);    
    
    if(color.isValid())
    {
 	QTextCharFormat fmt;
        fmt.setForeground(color);    
        mergeFormat(fmt);
    }
}


//  设置光标的选区，使格式作用于选区内的字符，若没有选区则作用于光标所在处的字符
void FontSet::mergeFormat(QTextCharFormat format) 	
{
    QTextCursor cursor = text->textCursor();
    if (!cursor.hasSelection())
       cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
   	text->mergeCurrentCharFormat(format);
}

void FontSet::slotCurrentFormatChanged(const QTextCharFormat &fmt)
{
    fontBox->setCurrentIndex(fontBox->findText(fmt.fontFamily()));
    sizeBox->setCurrentIndex(sizeBox->findText(QString::number(fmt.fontPointSize())));
    boldBtn->setChecked(fmt.font().bold());
    italicBtn->setChecked(fmt.fontItalic());
    underBtn->setChecked(fmt.fontUnderline());
}
