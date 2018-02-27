#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include "QLabel"
#include "QComboBox"
#include "QSpinBox"
#include "QCheckBox"
#include "QGridLayout"
#include "QPushButton"
#include "path.h"
#include "QSignalMapper"

class dashBoard : public QWidget
{
    Q_OBJECT
public:
    explicit dashBoard(QWidget *parent = nullptr, Path * sets [] = nullptr, int n = 0);

private:
    int numDays;
    LocationWidget * select1;
    LocationWidget * select2;
    int currSelection; //currently selecting #1 or #2 locationwidget
    vector<Path *> sets;
    vector<QLabel *> setLabels;
    vector<QCheckBox *> setCheckBoxes;
    vector<QPushButton *> locationButton;
    vector<QLabel *> coordinatesLabels;
    vector<QLabel *> addressLabels;
signals:

public slots:
    void togglePath(int pathNum);
    void toggleLocation(int pathNum, int locationNum);
    void swapLocations();
};

#endif // DASHBOARD_H
