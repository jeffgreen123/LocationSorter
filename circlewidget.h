#ifndef CIRCLEWIDGET_H
#define CIRCLEWIDGET_H
#include "QWidget"

class CircleWidget:public QWidget
{
private:
    float x,y;
    int windowSize;
    QBrush color;
public:
    CircleWidget(QWidget *parent,float x, float y, int windowSize, QBrush color = Qt::black);
    void setColor(QBrush newcolor);
protected:
    void mousePressEvent(QMouseEvent*) override;
    void paintEvent(QPaintEvent *event) override;


};

#endif // CIRCLEWIDGET_H
