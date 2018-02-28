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
#include <QGeoCoordinate>
#include <QGeoAddress>
#include <QGeoLocation>
#include <QApplication>
#include <QGeoAddress>
#include <QGeoCodingManager>
#include <QGeoCoordinate>
#include <QGeoLocation>
#include <QGeoServiceProvider>
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
int windowHeight = 682;
int windowWidth = 1400;
int dashBoardSize = 400;
float maxX = -78.365;
float minX = -80.51;
float maxY = 43.52;
float minY = 44.275;
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
               // cout << "SWAP" << endl;
                //cout << p1->otherPoints.at(i)->getX() << "," << p1->otherPoints.at(i)->getY() << endl;
                //cout << "AND" << endl;
               // cout << p2->otherPoints.at(j)->getX() << "," << p2->otherPoints.at(j)->getY() << endl;
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
        //cout << "PATH " << i << endl;
        for(int j = 0; j < sets[i]->otherPoints.size(); j++) {
            //sets[i]->otherPoints.at(j)->printInfo();
            sets[i]->otherPoints.at(j)->setColor(pathColors[i]);
            //cout << sets[i].otherPoints.at(j).second.latitude << "," << sets[i].otherPoints.at(j).second.longtitude << " " ;
        }
        //cout << endl;
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

    //get the long and lat for each address in the addresses.txt file
    string LongLatfilename = "/home/jgreen/LocationSorter/addressesOut.txt";
    string command = "python /home/jgreen/LocationSorter/addressConverter.py ";
    system(command.c_str());
    extra = 0;
    string line;

    vector<LocationWidget *> locations;
    ifstream myfile (LongLatfilename);
    if (myfile.is_open()){
        while ( getline (myfile,line) ) {
        float longitude = stof(line.substr(0,line.find(',')));
        float latitude = stof(line.substr(line.find(',') + 1,line.length() - 1));
        extra++;
        latitude = (latitude - minX)/abs(maxX - minX)*windowWidth;
        longitude = (longitude - minY)/abs(maxY - minY)*windowHeight;
         LocationWidget * newLoc = new LocationWidget(&window1,latitude,longitude, windowWidth, windowHeight);
         newLoc->setGeometry(0 + latitude - 50,0 - longitude - 50, 100, 100);
         locations.push_back(newLoc);
        }
        myfile.close();
    }





    vector<LocationWidget *> starts;
    vector<LocationWidget *> stops;

    Path * sets [numDays];

    LocationWidget *s1 = new LocationWidget(&window1,44.2323013,-79.3574353,windowWidth, windowHeight,Qt::green);
    starts.push_back(s1);

    LocationWidget *f1= new LocationWidget(&window1,44.4323013,-79.6574353,windowWidth, windowHeight,Qt::red);
    stops.push_back(f1);

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
