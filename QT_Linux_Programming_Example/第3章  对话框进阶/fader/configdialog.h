#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QPointer>
#include "fader.h"

class QListWidget;
class QListWidgetItem;
class QStackedWidget;

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    ConfigDialog();

public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private:
    QListWidget *contentsWidget;
    QStackedWidget *pagesWidget;
    QPointer<FaderWidget> faderWidget;
    
private slots:
    void fadeInWidget(int index);

};

#endif
