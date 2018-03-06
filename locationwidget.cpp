#include "locationwidget.h"
#include "dashboard.h"
using namespace std;
LocationWidget::LocationWidget(QWidget *parent,float x, float y,int windowWidth, int windowHeight,dashBoard * dBoard, QBrush color) :
    QWidget(parent), x(x),y(y),windowHeight(windowHeight),windowWidth(windowWidth),color(color), dBoard(dBoard),bHighlighted(false){

    radians = atan2(y + windowHeight/4,x - windowWidth/2);

}
LocationWidget::LocationWidget(const LocationWidget& l) {
}

void LocationWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    if(bHighlighted == true) {
        painter.setPen(QPen(Qt::red, 12, Qt::SolidLine, Qt::RoundCap));
        painter.drawPoint(6,6);
    }
    else {
        painter.setPen(QPen(color, 12, Qt::SolidLine, Qt::RoundCap));
        painter.drawPoint(6,6);
    }
}

void LocationWidget::mousePressEvent(QMouseEvent*){

    dBoard->toggleLocation(this);
    repaint();

}

void LocationWidget::setColor(QBrush newColor) {
    color = newColor;
    repaint();
}

void LocationWidget::setAddress(QString newAddress) {
    address = newAddress;
}

void LocationWidget::setCoordinates(float newX, float newY) {
    x = newX;
    y = newY;
    repaint();
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

const QBrush LocationWidget::getColor() {
    return color;
}

const QString LocationWidget::getAddress() {
    return address;
}

void LocationWidget::printInfo() {
    cout << x << "," << y << endl;
}

void LocationWidget::highlight() {
    bHighlighted = !bHighlighted;
    repaint();
}
