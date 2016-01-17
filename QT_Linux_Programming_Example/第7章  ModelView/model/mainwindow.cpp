#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{    
    setWindowTitle(tr("Custom Model"));
   
    createMenu(); 

    setupModel();
    setupView(); 
}

void MainWindow::setupModel()
{
    QStringList strList;
    model = new StringListModel(strList,this);
}

void MainWindow::setupView()
{
    list = new QListView;
    list->setModel(model);
    
    setCentralWidget(list);
}

void MainWindow::createMenu()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("File"));
    
    QAction *openAct = new QAction(tr("Open"),this);
    connect(openAct,SIGNAL(triggered()),this,SLOT(slotOpenFile()));
    fileMenu->addAction(openAct);
    
    QAction *saveAct = new QAction(tr("Save"),this);
    connect(saveAct,SIGNAL(triggered()),this,SLOT(slotSaveFile()));
    fileMenu->addAction(saveAct);
    
    QMenu *editMenu = menuBar()->addMenu(tr("Edit"));
    
    QAction *insertAct = new QAction(tr("Insert"),this);
    connect(insertAct,SIGNAL(triggered()),this,SLOT(slotInsertRows()));
    editMenu->addAction(insertAct);
    
    QAction *removeAct = new QAction(tr("Remove"),this);
    connect(removeAct,SIGNAL(triggered()),this,SLOT(slotRemoveRows()));
    editMenu->addAction(removeAct);
}

void MainWindow::slotOpenFile()
{
    
    name = QFileDialog::getOpenFileName(
        	this, 
        	"open file dialog",
        	".",
        	"strip files (*.txt)"); 
    
    if (!name.isEmpty())
    	openFile(name);
}

void MainWindow::slotSaveFile()
{
    if (name.isEmpty())
    	return;
    
    QFile file(name);
    if (!file.open(QFile::WriteOnly))
         return;
         
    QTextStream ts(&file);
    
    for(int i=0; i<model->rowCount();i++)
    {
    	QModelIndex index = model->index(i);
    	QString str = model->data(index,Qt::DisplayRole).toString();
    	ts << str << ",";
    }
}

void MainWindow::openFile(QString path)
{
    if (!path.isEmpty())
    {
    	QFile file(path);

         if (file.open(QFile::ReadOnly | QFile::Text)) 
         {
             QTextStream stream(&file);
             QString line;

             model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());

             int pos = 0;

             line = stream.readLine();
             if (!line.isEmpty()) 
             {
                 QStringList pieces = line.split(",", QString::SkipEmptyParts);
                 QString str;
                 foreach(str,pieces)
                 {
              	     model->insertRows(pos, 1, QModelIndex());
                     model->setData(model->index(pos),str);
                     pos++;
                 }                    
             }
             file.close();
         }
    } 
}

void MainWindow::slotInsertRows()
{
    bool ok;
    QModelIndex index = list->currentIndex();
    int rows = QInputDialog::getInteger(this,tr("Insert Row Number"),
    				tr("Please input number:"),1,1,10,1,&ok);
    if (ok)
    {
    	model->insertRows(index.row(),rows,QModelIndex());
    }
}

void MainWindow::slotRemoveRows()
{
    QModelIndex index = list->currentIndex();
 
    model->removeRows(index.row(),1,QModelIndex());
}





