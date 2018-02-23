#include "locationwidget.h"
#include "QPen"
#include "QPainter"
#include "math.h"
#include "string.h"
#include <iostream>
using namespace std;
LocationWidget::LocationWidget(QWidget *parent,float x, float y, int windowSize, QBrush color) :
    QWidget(parent), x(x),y(y),windowSize(windowSize),color(color){
    radians = atan2(y,x);
    this->setGeometry(windowSize/2 + x*4, windowSize/2 - y*4,100,100);
    bHighlighted = false;

}
LocationWidget::LocationWidget(const LocationWidget& l) {
}

void LocationWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    if(bHighlighted == true) {
        painter.setPen(QPen(color, 20, Qt::SolidLine, Qt::RoundCap));
        painter.drawPoint(50,50);
    }
    else {
        painter.setPen(QPen(color, 8, Qt::SolidLine, Qt::RoundCap));
        painter.drawPoint(50,50);
    }
}

void LocationWidget::mousePressEvent(QMouseEvent*){

    this->setColor(Qt::red);
    repaint();

}

void LocationWidget::setColor(QBrush newColor) {
    color = newColor;
    repaint();
}

void LocationWidget::setAddress(QString newAddress) {
    address = newAddress;
}

const float LocationWidget::getRadians() {
       return radians;
}

const float LocationWidget::getX() {
       return x;
}
const float LocationWidget::getY() {
       return y;
}

void LocationWidget::printInfo() {
    cout << x << "," << y << endl;
}

void LocationWidget::highlight() {
    bHighlighted = !bHighlighted;
    repaint();
}
