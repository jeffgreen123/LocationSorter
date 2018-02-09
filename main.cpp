#include <QApplication>
#include <QPushButton>
#include "locationwidget.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;



typedef std::pair<float,float> longLatPair;

typedef std::pair<float,int> distancePathPair;
bool distancePairComparator ( const distancePathPair& l, const distancePathPair& r)
   { return l.first > r.first; }

bool locationRadiansComparator (LocationWidget*  l,  LocationWidget * r)
   { return l->getRadians() > r->getRadians(); }

struct Path {
    LocationWidget *start;
    LocationWidget *stop;
    vector <LocationWidget * > otherPoints;
};

int numDays = 5; //number of unqiue paths (days of work)
int extra = 30; //number of outlying points
int numPerDay = 7; //number of extra jobs per day
int windowSize = 800;
QBrush pathColors [7];
float getAverageRadians(vector<LocationWidget *> locations) {
    float average = 0;
    for(int i = 0; i < locations.size(); i++) {
        average += locations.at(i)->getRadians();
    }
    average = average/locations.size();
    return average;
}

float getDistanceBetweenPoints(LocationWidget * l1, LocationWidget * l2) {
    float distX = fabs(l1->getX() - l2->getX());
    float distY = fabs(l1->getY() - l2->getY());
    //cout << l1.latitude << ',' <<  l1.longtitude <<  endl;
    return sqrtf(pow(distX,2) + pow(distY,2));
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
void adjustUnevenDays(Path sets[]) {
    float averages[numDays];
    for(int i = 0; i < numDays; i++) {
        averages[i] = getAverageRadians(sets[i].otherPoints);
    }
    for(int i = numDays - 2; i > 0; i--) {
        for( int j = 0; j < sets[i].otherPoints.size(); j++) {
            //cout << i << endl;
            if(fabs(sets[i].otherPoints.at(j)->getRadians() - averages[i + 1]) <= fabs(sets[i].otherPoints.at(j)->getRadians() - averages[i])
                && sets[i + 1].otherPoints.size() < numPerDay) {
                sets[i + 1].otherPoints.push_back(sets[i].otherPoints.at(j));
                sets[i].otherPoints.erase(sets[i].otherPoints.begin() + j);
                j -= 1;
            }
        }
    }
}


// attempt to swap any locations that are closer to the elements in another path
void adjustSwaps(Path& p1, Path& p2) {
    for(int i = 0; i < p1.otherPoints.size(); i++) {
        for(int j = 0; j < p2.otherPoints.size(); j++) {
            float averageL2P1Dist = getAverageDistance(p1.otherPoints, p2.otherPoints.at(j),j);
            float averageL2P2Dist = getAverageDistance(p2.otherPoints, p2.otherPoints.at(j),j);
            float averageL1P1Dist = getAverageDistance(p1.otherPoints, p1.otherPoints.at(i),i);
            float averageL1P2Dist = getAverageDistance(p2.otherPoints, p1.otherPoints.at(i),i);

            if((averageL1P1Dist - averageL1P2Dist) + (averageL2P2Dist - averageL2P1Dist) >= 0) {
                cout << "SWAP" << endl;
                cout << p1.otherPoints.at(i)->getX() << "," << p1.otherPoints.at(i)->getY() << endl;
                cout << "AND" << endl;
                cout << p2.otherPoints.at(j)->getX() << "," << p2.otherPoints.at(j)->getY() << endl;
                LocationWidget * temp = p2.otherPoints.at(j);
                p2.otherPoints.at(j) = p1.otherPoints.at(i);
                p1.otherPoints.at(i) = temp;
            }
        }
     }
}

void travellingSalesman(Path set, int numJobs) {
    //cout << numJobs <<endl;
    int finalPath[numJobs];
    int order [numJobs];
    for(int i = 0; i < numJobs; i++){
        order[i] = i;
        finalPath[i] = i;
    }
    float minDistance = 1000000000;
    do {

        float distance = getDistanceBetweenPoints(set.start,set.otherPoints.at(order[0])) +
            getDistanceBetweenPoints(set.stop,set.otherPoints.at(order[numJobs-1]));
        for(int i = 0; i < numJobs - 1; i++) {
            distance += getDistanceBetweenPoints(set.otherPoints.at(order[i]),set.otherPoints.at(order[i + 1]));
        }
        if(distance <= minDistance){
            for(int i = 0; i < numJobs; i++){
                finalPath[i] = order[i];
            }
            minDistance = distance;
        }
  } while ( std::next_permutation(order, order + numJobs) );

  cout << "DISTANCE:"  << minDistance << endl;
  cout << set.start->getX() << ',' << set.start->getY() << " to ";

  for( int i = 0; i < numJobs; i++) {
      cout << set.otherPoints.at(finalPath[i])->getX() << ',' << set.otherPoints.at(finalPath[i])->getY() << " to ";
  }

  cout << set.stop->getX() << ',' << set.stop->getY() << endl;
}

void divideLocations(vector <LocationWidget *> locations,vector <LocationWidget *> starts, vector <LocationWidget *> stops) {
    Path sets [numDays];


    int currSet = 0; //current path being filled
    while(!locations.empty() && currSet < numDays) {
        sets[currSet].otherPoints.push_back(locations.back());
        locations.pop_back();
        if(sets[currSet].otherPoints.size() == numPerDay) {
            currSet++;
        }
    }

    adjustUnevenDays(sets); // if didnt fill all days, see if we can push some into the last set to minimize distance

    for(int i = 0; i < numDays; i++) {
        for(int j = i + 1; j < numDays; j++) {
            adjustSwaps(sets[i],sets[j]); // swap the days that belong in diffrent path based on average distance
        }
    }

    for(int i = 0; i < numDays; i++) {
        sets[i].start = starts.at(0);
        sets[i].stop = stops.at(0);
        travellingSalesman(sets[i],sets[i].otherPoints.size());
    }

    for(int i = 0; i < numDays; i++) {
        cout << "PATH " << i << endl;
        for(int j = 0; j < sets[i].otherPoints.size(); j++) {
            sets[i].otherPoints.at(j)->printInfo();
            sets[i].otherPoints.at(j)->setColor(pathColors[i]);
            //cout << sets[i].otherPoints.at(j).second.latitude << "," << sets[i].otherPoints.at(j).second.longtitude << " " ;
        }
        cout << endl;
    }
}
int main(int argc, char **argv)
{
 QApplication app (argc, argv);
 QWidget window;
 pathColors[0] = Qt::blue;
 pathColors[1] = Qt::yellow;
 pathColors[2] = Qt::darkCyan;
 pathColors[3] = Qt::gray;
 pathColors[4] = Qt::cyan;
 pathColors[5] = Qt::magenta;
 pathColors[6] = Qt::darkGreen;
 window.setFixedSize(windowSize, windowSize);
 vector<LocationWidget *> locations;
 vector<LocationWidget *> starts;
 vector<LocationWidget *> stops;
 int XYvals[30][2] = {{-52,1},{-45,1},{-44,3},{-38,4},{-22,2},{-15,3}, {-12,2},{-8,1}, {-6,4},
                      {-49,51},{-48,52},{-46,54},{-27,51},
                      {5,23},{4,33},{3,35},{4,44},{2,45},{3,55},
                      {33,33},{45,45},{50,50},{52,52},{66,45},
                      {35,3},{49,2},{50,3},{50,1},{52,2},{53,3}};


 LocationWidget * points[30];

 LocationWidget *s1 = new LocationWidget(&window,1,2,windowSize,Qt::green);
 starts.push_back(s1);

 LocationWidget *f1= new LocationWidget(&window,5,3,windowSize,Qt::red);
 stops.push_back(f1);

 for( int i = 0; i < 30; i++){
     points[i] = new LocationWidget(&window,XYvals[i][0],XYvals[i][1], windowSize);
     points[i]->setGeometry(400 + XYvals[i][0]*4,400 - XYvals[i][1]*4, 8, 8);
     locations.push_back(points[i]);
 }
 for( int i = 0; i < 30; i++){
     cout << locations.at(i)->getRadians();
 }

 sort(locations.begin(),locations.end(),locationRadiansComparator);
divideLocations(locations,starts,stops);


 window.show();
 return app.exec();
}
