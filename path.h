#ifndef PATH_H
#define PATH_H

#include <QWidget>
#include "locationwidget.h"
#include <vector>

using namespace std;
class Path : public QWidget
{
    Q_OBJECT

private:
    LocationWidget *start;
    LocationWidget *stop;
    QBrush color;
public:
    vector <LocationWidget * > otherPoints;
    Path(QWidget *parent = nullptr, QBrush color = Qt::black);
    LocationWidget * getStart();
    void setStart(LocationWidget * l);
    LocationWidget * getStop();
    void setStop(LocationWidget * l);
    void setColor(QBrush newColor);
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // PATH_H
