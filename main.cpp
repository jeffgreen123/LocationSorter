#include <QApplication>
#include <QPushButton>
#include "locationwidget.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <string>
#include "path.h"
#include "dashboard.h"
using namespace std;



typedef std::pair<float,float> longLatPair;

typedef std::pair<float,int> distancePathPair;
bool distancePairComparator ( const distancePathPair& l, const distancePathPair& r)
   { return l.first > r.first; }

bool locationRadiansComparator (LocationWidget*  l,  LocationWidget * r)
   { return l->getRadians() > r->getRadians(); }


int numDays = 5; //number of unqiue paths (days of work)
int extra = 30; //number of outlying points
int numPerDay = 7; //number of extra jobs per day
int windowHeight = 761;
int windowWidth = 1563;
int dashBoardSize = 400;
QBrush pathColors [7];

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

void adjustUnevenDays(Path * sets[]) {
    float averages[numDays];
    for(int i = 0; i < numDays; i++) {
        averages[i] = getAverageRadians(sets[i]->otherPoints);
    }
    for(int i = numDays - 2; i > 0; i--) {
        for( int j = 0; j < sets[i]->otherPoints.size(); j++) {
            //cout << i << endl;
            if(fabs(sets[i]->otherPoints.at(j)->getRadians() - averages[i + 1]) <= fabs(sets[i]->otherPoints.at(j)->getRadians() - averages[i])
                && sets[i + 1]->otherPoints.size() < numPerDay) {
                sets[i + 1]->otherPoints.push_back(sets[i]->otherPoints.at(j));
                sets[i]->otherPoints.erase(sets[i]->otherPoints.begin() + j);
                j -= 1;
            }
        }
    }
}
// attempt to swap any locations that are closer to the elements in another path
void adjustSwaps(Path * p1, Path * p2) {
    for(int i = 0; i < p1->otherPoints.size(); i++) {
        for(int j = 0; j < p2->otherPoints.size(); j++) {
            float averageL2P1Dist = getAverageDistance(p1->otherPoints, p2->otherPoints.at(j),j);
            float averageL2P2Dist = getAverageDistance(p2->otherPoints, p2->otherPoints.at(j),j);
            float averageL1P1Dist = getAverageDistance(p1->otherPoints, p1->otherPoints.at(i),i);
            float averageL1P2Dist = getAverageDistance(p2->otherPoints, p1->otherPoints.at(i),i);

            if((averageL1P1Dist - averageL1P2Dist) + (averageL2P2Dist - averageL2P1Dist) >= 0) {
                cout << "SWAP" << endl;
                cout << p1->otherPoints.at(i)->getX() << "," << p1->otherPoints.at(i)->getY() << endl;
                cout << "AND" << endl;
                cout << p2->otherPoints.at(j)->getX() << "," << p2->otherPoints.at(j)->getY() << endl;
                LocationWidget * temp = p2->otherPoints.at(j);
                p2->otherPoints.at(j) = p1->otherPoints.at(i);
                p1->otherPoints.at(i) = temp;
            }
        }
     }
}

void divideLocations(vector <LocationWidget *> locations,vector <LocationWidget *> starts, vector <LocationWidget *> stops, Path * sets []) {


    int currSet = 0; //current path being filled
    while(!locations.empty() && currSet < numDays) {
        sets[currSet]->otherPoints.push_back(locations.back());
        locations.pop_back();
        if(sets[currSet]->otherPoints.size() == numPerDay) {
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
        sets[i]->setStart(starts.at(0));
        sets[i]->setStop(stops.at(0));
        sets[i]->travellingSalesman();
    }

    for(int i = 0; i < numDays; i++) {
        cout << "PATH " << i << endl;
        for(int j = 0; j < sets[i]->otherPoints.size(); j++) {
            sets[i]->otherPoints.at(j)->printInfo();
            sets[i]->otherPoints.at(j)->setColor(pathColors[i]);
            //cout << sets[i].otherPoints.at(j).second.latitude << "," << sets[i].otherPoints.at(j).second.longtitude << " " ;
        }
        cout << endl;
    }
}

int main(int argc, char **argv)
{
 QApplication app (argc, argv);
 QWidget window1;
 QWidget window2;
 pathColors[0] = Qt::blue;
 pathColors[1] = Qt::yellow;
 pathColors[2] = Qt::darkCyan;
 pathColors[3] = Qt::gray;
 pathColors[4] = Qt::cyan;
 pathColors[5] = Qt::magenta;
 pathColors[6] = Qt::darkGreen;
 window1.setFixedSize(windowWidth, windowHeight);
 window2.setFixedSize(dashBoardSize, windowHeight);
 vector<LocationWidget *> locations;
 vector<LocationWidget *> starts;
 vector<LocationWidget *> stops;
 int XYvals[30][2] = {{-52,1},{-45,1},{-44,3},{-38,4},{-22,2},{-15,3}, {-12,2},{-8,1}, {-6,4},
                      {-49,51},{-48,52},{-46,54},{-27,51},
                      {5,23},{4,33},{3,35},{4,44},{2,45},{3,55},
                      {33,33},{45,45},{50,50},{52,52},{66,45},
                      {35,3},{49,2},{50,3},{50,1},{52,2},{53,3}};

 Path * sets [numDays];
 LocationWidget * points[30];
 LocationWidget *s1 = new LocationWidget(&window1,1,2,windowWidth, windowHeight,Qt::green);
 starts.push_back(s1);

 LocationWidget *f1= new LocationWidget(&window1,5,3,windowWidth, windowHeight,Qt::red);
 stops.push_back(f1);

 for( int i = 0; i < 30; i++){
     points[i] = new LocationWidget(&window1,XYvals[i][0],XYvals[i][1], windowWidth, windowHeight);
     points[i]->setGeometry(windowWidth/2 + XYvals[i][0]*4,windowHeight/2 - XYvals[i][1]*4, 100, 100);
     locations.push_back(points[i]);
 }

 sort(locations.begin(),locations.end(),locationRadiansComparator);


 for(int i = 0; i < numDays; i++) {
     sets[i] = new Path(&window1,pathColors[i],true,windowWidth,windowHeight);
     sets[i]->setGeometry(0,0,windowWidth, windowHeight);
 }

 divideLocations(locations,starts,stops,sets);
 QPixmap bkgnd("/home/jgreen/LocationSorter/GTA.png");
 //bkgnd = bkgnd.scaled(windowSize,windowSize,Qt::KeepAspectRatio);
 QPalette palette;
 palette.setBrush(QPalette::Background, bkgnd);
 window1.setPalette(palette);

 dashBoard * d = new dashBoard(&window2,sets, numDays);
 d->setGeometry(0,0, dashBoardSize, windowHeight);



 window1.show();
 window2.show();
 return app.exec();
}
