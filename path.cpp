#include "path.h"
#include "QPainter"
#include "QGraphicsView"

Path::Path(QWidget *parent, QBrush color, bool bShowPath) : QWidget(parent), color(color), bShowPath(bShowPath) {

}

LocationWidget* Path::getStart(){
    return start;
}

void Path::setStart(LocationWidget * l){
    start = l;
}

LocationWidget* Path::getStop(){
    return stop;
}

void Path::setStop(LocationWidget * l){
    stop = l;
}

void Path::setColor(QBrush newColor) {
    color = newColor;
    repaint();
}

void Path::setShowPath(bool newShowPath) {
    bShowPath = newShowPath;
    repaint();
}

void Path::mousePressEvent(QMouseEvent* event){
    bShowPath = false;
    repaint();
    //QGraphicsView::mousePressEvent(event);
}
void Path::paintEvent(QPaintEvent *event){
    if(bShowPath) {
        QPainter painter(this);
        painter.setPen(QPen(color, 3, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(400 + start->getX()*4 + 4,400 - start->getY()*4 + 4,400 + otherPoints.at(0)->getX()*4 + 4,400 - otherPoints.at(0)->getY()*4 + 4);
        for(int i = 0; i < otherPoints.size() - 1; i++) {
            painter.drawLine(400 + otherPoints.at(i)->getX()*4 + 4,400 - otherPoints.at(i)->getY()*4 + 4,400 + otherPoints.at(i+1)->getX()*4 + 4,400 - otherPoints.at(i+1)->getY()*4 + 4);
        }
        painter.drawLine(400 + stop->getX()*4 + 4,400 - stop->getY()*4 + 4,400 + otherPoints.at(otherPoints.size() - 1)->getX()*4 + 4,400 - otherPoints.at(otherPoints.size() - 1)->getY()*4 + 4);
    }
}
