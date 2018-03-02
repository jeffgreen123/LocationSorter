#ifndef PATH_H
#define PATH_H

#include <QWidget>
#include "locationwidget.h"
#include "helperfunctions.h"
#include <vector>
#include "QPainter"
#include "QGraphicsView"
#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <string>

using namespace std;
class Path : public QWidget
{
    Q_OBJECT

private:
    LocationWidget *start;
    LocationWidget *stop;
    QBrush color;
    int windowWidth;
    int windowHeight;
    bool bShowPath;
public:
    vector <LocationWidget * > otherPoints;
    Path(QWidget *parent = nullptr, QBrush color = Qt::black, bool bShowPath = false, int windowWidth = 0, int windowHeight = 0);
    LocationWidget * getStart();
    void setStart(LocationWidget * l);
    LocationWidget * getStop();
    void setStop(LocationWidget * l);
    void setColor(QBrush newColor);
    void setShowPath(bool newShowPath);
    void travellingSalesman();
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // PATH_H
