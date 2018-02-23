#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include "QLabel"
#include "QComboBox"
#include "QSpinBox"
#include "QCheckBox"
#include "QGridLayout"
#include "path.h"
#include "QSignalMapper"

class dashBoard : public QWidget
{
    Q_OBJECT
public:
    explicit dashBoard(QWidget *parent = nullptr, Path * sets [] = nullptr, int n = 0);

private:
    int numDays;
    vector<Path *> sets;
    vector<QLabel *> setLabels;
    vector<QCheckBox *> setCheckBoxes;
    QComboBox *shapeComboBox;
    QSpinBox *penWidthSpinBox;
    QComboBox *penStyleComboBox;
    QComboBox *penCapComboBox;
    QComboBox *penJoinComboBox;
    QComboBox *brushStyleComboBox;
    QCheckBox *antialiasingCheckBox;
    QCheckBox *transformationsCheckBox;
signals:

public slots:
    void togglePath(int pathNum);
};

#endif // DASHBOARD_H
