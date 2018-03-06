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

int numDays = 5; //number of unqiue paths (days of work)
int numPerDay = 6; //number of extra jobs per day
int windowHeight = 682;
int windowWidth = 1400;
int dashBoardSize = 400;
float maxX = -78.365;
float minX = -80.51;
float maxY = 43.52;
float minY = 44.275;

QBrush pathColors [7] ={Qt::blue,Qt::yellow,Qt::darkCyan,Qt::gray,Qt::cyan,Qt::magenta,Qt::darkGreen};

typedef std::pair<float,float> longLatPair;

typedef std::pair<float,int> distancePathPair;
bool distancePairComparator ( const distancePathPair& l, const distancePathPair& r)
   { return l.first > r.first; }

bool locationRadiansComparator (LocationWidget*  l,  LocationWidget * r)
   { return l->getRadians() > r->getRadians(); }


// attempt to swap any locations that are closer to the elements in another path
void adjustSwaps(Path * p1, Path * p2) {
    for(int i = 0; i < p1->otherPoints.size(); i++) {
        for(int j = 0; j < p2->otherPoints.size(); j++) {
            float averageL2P1Dist = getAverageDistance(p1->otherPoints, p2->otherPoints.at(j),-1);
            float averageL2P2Dist = getAverageDistance(p2->otherPoints, p2->otherPoints.at(j),j);
            float averageL1P1Dist = getAverageDistance(p1->otherPoints, p1->otherPoints.at(i),i);
            float averageL1P2Dist = getAverageDistance(p2->otherPoints, p1->otherPoints.at(i),-1);

            if((averageL1P1Dist - averageL1P2Dist) + (averageL2P2Dist - averageL2P1Dist) >= 0) {
                LocationWidget * temp = p2->otherPoints.at(j);
                p2->otherPoints.at(j) = p1->otherPoints.at(i);
                p1->otherPoints.at(i) = temp;
                return;
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

    for(int k = 0; k < 100; k++){
        for(int i = 0; i < numDays; i++) {
            for(int j = i + 1; j < numDays; j++) {
                adjustSwaps(sets[i],sets[j]); //swap some of the elements that were not placed in the correct location
            }
        }
    }

    for(int i = 0; i < numDays; i++) {
        sets[i]->setStart(starts.at(0));
        sets[i]->setStop(stops.at(0));
        sets[i]->travellingSalesman();
    }

    for(int i = 0; i < numDays; i++) {
        for(int j = 0; j < sets[i]->otherPoints.size(); j++) {
            sets[i]->otherPoints.at(j)->setColor(pathColors[i]);
        }
    }
}

int main(int argc, char **argv)
{

    QApplication app (argc, argv);
    QWidget window1;
    QWidget window2;



    window1.setFixedSize(windowWidth, windowHeight);
    window2.setFixedSize(dashBoardSize, windowHeight);


    Path * sets [numDays];
    for(int i = 0; i < numDays; i++) {
        sets[i] = new Path(&window1,pathColors[i],true,windowWidth,windowHeight);
        sets[i]->setGeometry(0,0,windowWidth, windowHeight);
    }

    dashBoard * dBoard = new dashBoard(&window2,sets, numDays);
    dBoard->setGeometry(0,0, dashBoardSize, windowHeight);
    //get the long and lat for each address in the addresses.txt file

    string LongLatfilename = "/home/jgreen/LocationSorter/addressesOut.txt";
    string command = "python /home/jgreen/LocationSorter/addressConverter.py ";
    system(command.c_str());
    int extra = 0; //extra number of days

    string line;
    vector<LocationWidget *> locations;
    ifstream myfile (LongLatfilename);
    if (myfile.is_open()){
        while ( getline (myfile,line) ) {
            float longitude = stof(line.substr(0,line.find(',')));
            float latitude = stof(line.substr(line.find(',') + 1,line.find('=')));
            QString address = QString::fromStdString(line.substr(line.find('=') + 1,line.length() - 1));

            extra++;
            latitude = (latitude - minX)/abs(maxX - minX)*windowWidth;
            longitude = (longitude - minY)/abs(maxY - minY)*windowHeight;
            LocationWidget * newLoc = new LocationWidget(&window1,latitude,longitude, windowWidth, windowHeight,dBoard, Qt::black);
            newLoc->setAddress(address);
            newLoc->setGeometry(0 + latitude - 6,0 - longitude - 6, 12, 12);
            locations.push_back(newLoc);
        }
        myfile.close();
    }

    vector<LocationWidget *> starts;
    vector<LocationWidget *> stops;

    //temp start and finish values
    LocationWidget *s1 = new LocationWidget(&window1,800,-100,windowWidth, windowHeight,dBoard,Qt::green);
    starts.push_back(s1);

    LocationWidget *f1= new LocationWidget(&window1,800,-100,windowWidth, windowHeight,dBoard, Qt::red);
    stops.push_back(f1);

    sort(locations.begin(),locations.end(),locationRadiansComparator);

    divideLocations(locations,starts,stops,sets);

    QPixmap bkgnd("/home/jgreen/LocationSorter/GTA.png");
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    window1.setPalette(palette);




    window1.show();
    window2.show();



    return app.exec();
}
