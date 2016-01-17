#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "stringlistmodel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent=0);
    
    void createMenu();
    void setupModel();
    void setupView();
    void openFile(QString);
    
public slots:
    void slotOpenFile();
    void slotSaveFile();
    void slotInsertRows();
    void slotRemoveRows();
    
private:  
    QListView *list;
    StringListModel *model;
    QString name;
};

#endif 	// MAINWINDOW_H
