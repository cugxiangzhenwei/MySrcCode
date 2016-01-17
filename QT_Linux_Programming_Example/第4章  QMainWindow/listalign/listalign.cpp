#include "listalign.h"

ListAlign::ListAlign(QWidget *parent)
	: QMainWindow(parent)
{
    QFont f("ZYSong18030",12);
    setFont(f);
    	
    setWindowTitle(tr("List&Alignment"));
  
    QToolBar *toolBar = addToolBar("List");
    
    QLabel *label = new QLabel(tr("List: "));
    listBox = new QComboBox(toolBar);
    listBox->addItem(tr("Standard"));
    listBox->addItem(tr("Bullet List (Disc)"));
    listBox->addItem(tr("Bullet List (Circle)"));
    listBox->addItem(tr("Bullet List (Square)"));
    listBox->addItem(tr("Ordered List (Decimal)"));
    listBox->addItem(tr("Ordered List (Alpha lower)"));
    listBox->addItem(tr("Ordered List (Alpha upper)"));
    toolBar->addWidget(label);
    toolBar->addWidget(listBox);

    toolBar->addSeparator();
    
    QActionGroup *actGrp = new QActionGroup(this);
    leftAction = new QAction(QIcon(":/images/left.png"),tr("left"),actGrp);
    leftAction->setCheckable(true);

    centerAction = new QAction(QIcon(":/images/center.png"),tr("center"),actGrp);
    centerAction->setCheckable(true);

    justifyAction = new QAction(QIcon(":/images/justify.png"),tr("justify"),actGrp);
    justifyAction->setCheckable(true);
    
    rightAction = new QAction(QIcon(":/images/right.png"),tr("right"),actGrp);
    rightAction->setCheckable(true);
    
    toolBar->addActions(actGrp->actions());
    
    QToolBar *editBar = addToolBar("Edit");
    undoAction = new QAction(QIcon(":/images/undo.png"),tr("undo"),this);
    editBar->addAction(undoAction);
    redoAction = new QAction(QIcon(":/images/redo.png"),tr("redo"),this);
    editBar->addAction(redoAction);
    editBar->addAction(undoAction);
    editBar->addAction(redoAction);

    text =  new QTextEdit(this);
    text->setFocus();
    setCentralWidget(text);
    
    connect(listBox,SIGNAL(activated(int)),this,SLOT(slotList(int)));
    connect(actGrp,SIGNAL(triggered(QAction*)),this,SLOT(slotAlignment(QAction*)));
    
    connect(redoAction,SIGNAL(triggered()),text,SLOT(redo()));
    connect(undoAction,SIGNAL(triggered()),text,SLOT(undo()));
    connect(text->document(),SIGNAL(redoAvailable(bool)),redoAction,SLOT(setEnabled(bool)));
    connect(text->document(),SIGNAL(undoAvailable(bool)),undoAction,SLOT(setEnabled(bool)));
    
    connect(text,SIGNAL(cursorPositionChanged()),this,SLOT(slotCursorPositionChanged()));
}

void ListAlign::slotAlignment(QAction *act)
{
    if (act == leftAction)
    	text->setAlignment(Qt::AlignLeft);
    if (act == centerAction)
    	text->setAlignment(Qt::AlignCenter);
    if (act == justifyAction)
    	text->setAlignment(Qt::AlignJustify);
    if (act == rightAction)
    	text->setAlignment(Qt::AlignRight);
}

void ListAlign::slotList(int index)
{
    QTextCursor cursor = text->textCursor();

     if (index != 0) 
     {
         QTextListFormat::Style style = QTextListFormat::ListDisc;

         switch (index) 
         {
             default:
             case 1:
                 style = QTextListFormat::ListDisc;
                 break;
             case 2:
                 style = QTextListFormat::ListCircle;
                 break;
             case 3:
                 style = QTextListFormat::ListSquare;
                 break;
             case 4:
                 style = QTextListFormat::ListDecimal;
                 break;
             case 5:
                 style = QTextListFormat::ListLowerAlpha;
                 break;
             case 6:
                 style = QTextListFormat::ListUpperAlpha;
                 break;
         }

         cursor.beginEditBlock();

         QTextBlockFormat blockFmt = cursor.blockFormat();

         QTextListFormat listFmt;

         if (cursor.currentList()) 
         {
             listFmt = cursor.currentList()->format();
         } 
         else 
         {
             listFmt.setIndent(blockFmt.indent() + 1);
             blockFmt.setIndent(0);
             cursor.setBlockFormat(blockFmt);
         }

         listFmt.setStyle(style);

         cursor.createList(listFmt);

         cursor.endEditBlock();
     } 
     else 
     {
         QTextBlockFormat bfmt;
         bfmt.setObjectIndex(-1);
         cursor.mergeBlockFormat(bfmt);
     }
}

void ListAlign::slotCursorPositionChanged()
{
    if (text->alignment() == Qt::AlignLeft)
    	leftAction->setChecked(true);
    if (text->alignment() == Qt::AlignCenter)
    	centerAction->setChecked(true);
    if (text->alignment() == Qt::AlignJustify)
    	justifyAction->setChecked(true);
    if (text->alignment() == Qt::AlignRight)
    	rightAction->setChecked(true);
}
