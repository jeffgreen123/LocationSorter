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

public:
    vector <LocationWidget * > otherPoints;
    explicit Path(QWidget *parent = nullptr);
    LocationWidget * getStart();
    void setStart(LocationWidget * l);
    LocationWidget * getStop();
    void setStop(LocationWidget * l);
    vector <LocationWidget * >  getPoints();
    void setPoint(int i, LocationWidget * l);

};

#endif // PATH_H
