#ifndef FILEINFORMATION_H
#define FILEINFORMATION_H

#include <QtGui>

class FileInformation : public QDialog	
{
    Q_OBJECT
public:
    FileInformation( QWidget *parent=0, Qt::WindowFlags  f=0 );
    ~FileInformation();
public:
	QLabel* LabelFileName;
	QLineEdit* LineEditFileName;
	QPushButton* PushButtonFile;
	QLabel* LabelSize;
	QLineEdit* LineEditSize;
	QLabel* LabelCreated;
	QLineEdit* LineEditCreated;
	QLabel* LabelLastModified ;
	QLineEdit* LineEditLastModified;		
	QLabel* LabelLastRead ;
	QLineEdit* LineEditLastRead;
	QLabel* LabelProperty ;			
	QCheckBox* CheckBoxIsDir;	
	QCheckBox* CheckBoxIsFile;
	QCheckBox* CheckBoxIsSymLink;
	QCheckBox* CheckBoxIsHidden;
	QCheckBox* CheckBoxIsReadable;	
	QCheckBox* CheckBoxIsWritable;	
	QCheckBox* CheckBoxIsExecutable;	
		
    QPushButton* PushButtonGet;
public:
	void getFileInformation(QString file);
public slots:
	void slotFile();
    void slotGet();
};


#endif
