#include "locationwidget.h"

using namespace std;
LocationWidget::LocationWidget(QWidget *parent,float x, float y,int windowWidth, int windowHeight, QBrush color) :
    QWidget(parent), x(x),y(y),windowHeight(windowHeight),windowWidth(windowWidth),color(color){
    radians = atan2(y,x);
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

void LocationWidget::setCoordinates(float x, float y) {
    this->x = x;
    this->y = y;
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
