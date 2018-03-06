
#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H
#include "locationwidget.h"
#include <vector>
using namespace std;
float getDistanceBetweenPoints(LocationWidget * l1, LocationWidget * l2);

float getAverageRadians(vector<LocationWidget *> locations);

float getAverageDistance(vector<LocationWidget *> locations, LocationWidget * point, int currIndex);
#endif // HELPERFUNCTIONS_H

