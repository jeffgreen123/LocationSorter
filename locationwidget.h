#ifndef LOCATIONWIDGET_H
#define LOCATIONWIDGET_H
#include "QWidget"
#include <string>
#include "QPen"
#include "QPainter"
#include "math.h"
#include "string.h"
#include <iostream>

class LocationWidget:public QWidget
{
    Q_OBJECT
private:
    float x,y;
    int windowWidth;
    int windowHeight;
    QBrush color;
    QString address;
    float radians;
    bool bHighlighted;
public:
    LocationWidget(QWidget *parent,float x, float y,int windowWidth, int windowHeight, QBrush color = Qt::black);
    LocationWidget(const LocationWidget& l);
    void setColor(QBrush newcolor);
    void setAddress(QString newAddress);
    void setCoordinates(float x, float y);
    const float getRadians();
    const float getX();
    const float getY();
    const QBrush getColor();
    const QString getAddress();
    void printInfo();
    void highlight();
protected:
    void mousePressEvent(QMouseEvent*) override;
    void paintEvent(QPaintEvent *event) override;


};

#endif // LOCATIONWIDGET_H
