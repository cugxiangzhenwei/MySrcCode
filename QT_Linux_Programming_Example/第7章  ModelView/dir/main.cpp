#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    QDirModel model;      
    QListView list;
    QTreeView tree;   
    QTableView table; 
    
    tree.setModel(&model);
    list.setModel(&model); 
    table.setModel(&model); 
    
    tree.setSelectionMode(QAbstractItemView::MultiSelection);    
    list.setSelectionModel(tree.selectionModel());
    table.setSelectionModel(tree.selectionModel());
     
    QObject::connect(&tree,SIGNAL(doubleClicked(QModelIndex)),&list,SLOT(setRootIndex(QModelIndex)));
    QObject::connect(&tree,SIGNAL(doubleClicked(QModelIndex)),&table,SLOT(setRootIndex(QModelIndex)));
    
    QSplitter *splitter = new QSplitter;    
    splitter->addWidget(&tree);
    splitter->addWidget(&list);
    splitter->addWidget(&table);  
    
    splitter->setWindowTitle(QObject::tr("Model/View"));
    splitter->show();
    
    return app.exec();
}

