#include "pararw.h"
#include <QtGui>

ParaRW::ParaRW()
{
    QFont f("ZYSong18030",12);
    setFont(f);
    
    setWindowTitle(tr("QDataStrem"));
    
    label1 = new QLabel(tr("Channel:"));
    label2 = new QLabel(tr("Power:"));
    label3 = new QLabel(tr("Frequency:"));
    label4 = new QLabel(tr("MHz"));
    label5 = new QLabel(tr("Last save time:"));
    timeLabel = new QLabel;
    
    saveButton = new QPushButton(tr("Save"));
    getButton = new QPushButton(tr("Get"));
    
    channelSpinBox = new QSpinBox;
    channelSpinBox->setRange(0,10);
    powerComboBox = new QComboBox;
    powerComboBox->insertItem(0,tr("small"));
    powerComboBox->insertItem(1,tr("mid"));
    powerComboBox->insertItem(2,tr("big"));
    frequencyEdit = new QLineEdit;
    
    QGridLayout *layout = new QGridLayout(this);
    layout->setMargin(20);
    layout->setSpacing(10);
    layout->addWidget(label1,0,0);
    layout->addWidget(channelSpinBox,0,1);
    layout->addWidget(label2,1,0);
    layout->addWidget(powerComboBox,1,1);
    layout->addWidget(label3,2,0);
    layout->addWidget(frequencyEdit,2,1);
    layout->addWidget(label4,2,2);
    layout->addWidget(label5,0,2);
    layout->addWidget(timeLabel,0,3);
    layout->addWidget(saveButton,1,3);
    layout->addWidget(getButton,2,3);
    
    connect(saveButton,SIGNAL(clicked()),this,SLOT(slotSave()));
    connect(getButton,SIGNAL(clicked()),this,SLOT(slotGet()));
}

void
ParaRW::slotSave()
{
    int channel = channelSpinBox->value();
    int power = powerComboBox->currentIndex();
    float frequency = frequencyEdit->text().toFloat();
    QDateTime *time = new QDateTime;
    
    QFile file("parameters.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)0xa1a2a3a4;
    
    out << (qint32)channel << (qint32)power << frequency << time->currentDateTime();
}

void
ParaRW::slotGet()
{
    QFile file("parameters.dat");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    
    in.setVersion(QDataStream::Qt_4_0);
    qint32 magic;
    in >> magic;
    if (magic != 0xa1a2a3a4)
    {
    	QMessageBox::information(this,"exception",tr("invalide file format"));
    	return;
    }
    
    qint32 channel;
    qint32 power;
    float frequency;
    QDateTime time;
    in >> channel >> power >> frequency >> time;
    
    channelSpinBox->setValue(channel);
    powerComboBox->setCurrentIndex(power);
    QString freq;
    frequencyEdit->setText(freq.setNum(frequency));
    QString lastTime = time.date().toString(Qt::ISODate) + " " + time.time().toString();
    timeLabel->setText(lastTime);
}

