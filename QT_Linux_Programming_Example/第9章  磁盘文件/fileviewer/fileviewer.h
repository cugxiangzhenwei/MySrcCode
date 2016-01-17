#ifndef FILEVIEWER_H
#define FILEVIEWER_H

#include <QtGui>

class FileViewer : public QDialog		// file information dialogs widget
{
    Q_OBJECT
public:
    FileViewer( QWidget *parent=0, Qt::WindowFlags  f=0 );
    ~FileViewer();
public:
	QLineEdit* LineEditDir;
	QListWidget* ListWidgetFile;

public:
	void showFileInfoList(QFileInfoList list);
public slots:
	void slotShowDir(QListWidgetItem * item);
	void slotShow(QDir dir);
};


#endif // FILEVIEWER_H
