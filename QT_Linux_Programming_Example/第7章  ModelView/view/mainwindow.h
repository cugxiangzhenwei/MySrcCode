#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

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
    
private:
    QStandardItemModel *model;
};

#endif 	// MAINWINDOW_H
