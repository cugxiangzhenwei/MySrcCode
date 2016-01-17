#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


// Ö÷´°¿Ú
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent=0);
    
    void createToolBars();
    void createStatusBar();
};

#endif // MAINWINDOW_H
