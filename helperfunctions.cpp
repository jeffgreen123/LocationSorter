#include "helperfunctions.h"

float getDistanceBetweenPoints(LocationWidget * l1, LocationWidget * l2) {
    float distX = fabs(l1->getX() - l2->getX());
    float distY = fabs(l1->getY() - l2->getY());
    //cout << l1.latitude << ',' <<  l1.longtitude <<  endl;
    return sqrtf(pow(distX,2) + pow(distY,2));
}
