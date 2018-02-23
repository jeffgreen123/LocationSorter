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
    bool bShowPath;
public:
    vector <LocationWidget * > otherPoints;
    Path(QWidget *parent = nullptr, QBrush color = Qt::black, bool bShowPath = false);
    LocationWidget * getStart();
    void setStart(LocationWidget * l);
    LocationWidget * getStop();
    void setStop(LocationWidget * l);
    void setColor(QBrush newColor);
    void setShowPath(bool newShowPath);
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent *event) override;
};

#endif // PATH_H
