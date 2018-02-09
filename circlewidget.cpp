#include "circlewidget.h"
#include "QPen"
#include "QPainter"

CircleWidget::CircleWidget(QWidget *parent,float x, float y, int windowSize, QBrush color) :
    QWidget(parent), x(x),y(y),windowSize(windowSize),color(color)
{

}

void CircleWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(QPen(color, 8, Qt::SolidLine, Qt::RoundCap));
   // painter.drawLine(windowSize/2,0,windowSize/2,windowSize);
   // painter.drawLine(0,windowSize/2,windowSize,windowSize/2);
    painter.setPen(QPen(color, 8, Qt::SolidLine, Qt::RoundCap));
    painter.drawPoint(x,y);
    //painter.drawEllipse(QRect(70, 200, 15, 200 + 5));
}
void CircleWidget::mousePressEvent(QMouseEvent*)
{
    this->setColor(Qt::red);
    repaint();

}

void CircleWidget::setColor(QBrush newColor) {
    color = newColor;
}
