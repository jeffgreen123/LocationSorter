#include "helperfunctions.h"

float getAverageRadians(vector<LocationWidget *> locations) {
    float average = 0;
    for(int i = 0; i < locations.size(); i++) {
        average += locations.at(i)->getRadians();
    }
    average = average/locations.size();
    return average;
}


float getAverageDistance(vector<LocationWidget *> locations, LocationWidget * point, int currIndex) {
    float average = 0;
    for(int i = 0; i < locations.size(); i++) {
        if( i != currIndex) {
            average += getDistanceBetweenPoints(locations.at(i), point);
        }
    }
    average = average/(locations.size() - 1);
    return average;
}

float getDistanceBetweenPoints(LocationWidget * l1, LocationWidget * l2) {
    float distX = fabs(l1->getX() - l2->getX());
    float distY = fabs(l1->getY() - l2->getY());
    //cout << l1.latitude << ',' <<  l1.longtitude <<  endl;
    return sqrtf(pow(distX,2) + pow(distY,2));
}
