#ifndef FADER_H
#define FADER_H

#include <QWidget>

class QColor;
class QTimer;

class FaderWidget : public QWidget
{
    Q_OBJECT

public:
    FaderWidget(QWidget *parent);
    
    void start();
    
protected:
    void paintEvent(QPaintEvent *event);
    
private:
    QTimer *timer;
    int currentAlpha;
    QColor startColor;
    int duration;
};

#endif  // FADER_H
