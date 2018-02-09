#ifndef LOCATIONWIDGET_H
#define LOCATIONWIDGET_H
#include "QWidget"
#include <string>


class LocationWidget:public QWidget
{
    Q_OBJECT
private:
    float x,y;
    int windowSize;
    QBrush color;
    QString address;
    float radians;
public:
    LocationWidget(QWidget *parent,float x, float y, int windowSize, QBrush color = Qt::black);
    LocationWidget(const LocationWidget& l);
    void setColor(QBrush newcolor);
    void setAddress(QString newAddress);
    const float getRadians();
    const float getX();
    const float getY();
    void printInfo();
protected:
    void mousePressEvent(QMouseEvent*) override;
    void paintEvent(QPaintEvent *event) override;


};

#endif // LOCATIONWIDGET_H
