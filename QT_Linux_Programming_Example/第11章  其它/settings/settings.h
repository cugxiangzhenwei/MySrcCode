#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

class QPushButton;
class QLabel;
class QTextEdit;

class Settings : public QDialog
{
    Q_OBJECT
public:
    Settings();
    
    QLabel *label;
    QPushButton *colorBtn;
    QTextEdit *edit;
   
    void readSettings();
    void writeSettings();
    
protected:
    void closeEvent(QCloseEvent *);
    
public slots:
    void slotColor();
};

#endif // SETTINGS_H
