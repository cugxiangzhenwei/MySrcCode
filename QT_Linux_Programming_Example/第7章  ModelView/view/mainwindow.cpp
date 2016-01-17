#include "mainwindow.h"
#include "histogramview.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{   
    setWindowTitle(tr("Custom View"));
   
    createMenu(); 

    setupModel();
    setupView(); 
    
    openFile("department.strip");   
    
    resize(600,600);
}

void MainWindow::setupModel()
{
    model = new QStandardItemModel(4,4,this);
    model->setHeaderData(0,Qt::Horizontal,tr("department"));
    model->setHeaderData(1,Qt::Horizontal,tr("male"));
    model->setHeaderData(2,Qt::Horizontal,tr("female"));
    model->setHeaderData(3,Qt::Horizontal,tr("retire"));
    
}

void MainWindow::setupView()
{
    QSplitter *splitter = new QSplitter;
    splitter->setOrientation(Qt::Vertical);
    QTableView *table = new QTableView;
    HistogramView *histogram = new HistogramView(splitter);
    
    table->setModel(model);
    histogram->setModel(model);

    QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    table->setSelectionModel(selectionModel);
    histogram->setSelectionModel(selectionModel);

    connect(selectionModel,SIGNAL(selectionChanged(const QItemSelection, const QItemSelection)),
    		histogram,SLOT(selectionChanged(const QItemSelection, const QItemSelection)));
    connect(selectionModel,SIGNAL(selectionChanged(const QItemSelection, const QItemSelection)),
    		table,SLOT(selectionChanged(const QItemSelection, const QItemSelection)));
    
    splitter->addWidget(table);
    splitter->addWidget(histogram);
    
    setCentralWidget(splitter);
}

void MainWindow::createMenu()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("File"));
    
    QAction *openAct = new QAction(tr("Open"),this);
    connect(openAct,SIGNAL(triggered()),this,SLOT(slotOpenFile()));
    fileMenu->addAction(openAct);
}

void MainWindow::slotOpenFile()
{
    QString name;
    name = QFileDialog::getOpenFileName(
        	this, 
        	"open file dialog",
        	"/",
        	"histogram files (*.strip)"); 
    
    if (!name.isEmpty())
    	openFile(name);
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

             int row = 0;
             do {
                 line = stream.readLine();
                 if (!line.isEmpty()) {

                     model->insertRows(row, 1, QModelIndex());

                     QStringList pieces = line.split(",", QString::SkipEmptyParts);
                     model->setData(model->index(row, 0, QModelIndex()),
                                    pieces.value(0));
                     model->setData(model->index(row, 1, QModelIndex()),
                                    pieces.value(1));
                     model->setData(model->index(row, 2, QModelIndex()),
                                    pieces.value(2));
                     model->setData(model->index(row,3, QModelIndex()),
                                    pieces.value(3));            
                     row++;
                 }
             } while (!line.isEmpty());

             file.close();

         }
    } 

}





