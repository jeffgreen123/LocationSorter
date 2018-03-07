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
#include <limits.h>
#include <unistd.h>
using namespace std;



// comparator of radians of location widgets
bool locationRadiansComparator (LocationWidget*  l,  LocationWidget * r)
   { return l->getRadians() > r->getRadians(); }


// attempt to swap any locations that are closer to the elements in a diffrent path
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

//divide the locations based on their radian values
void divideLocations(vector <LocationWidget *> locations,vector <LocationWidget *> starts, vector <LocationWidget *> stops, Path * sets [], int numDays, int numPerDay) {


    int currSet = 0; //current path being filled
    while(!locations.empty() && currSet < numDays) {
        sets[currSet]->otherPoints.push_back(locations.back());
        locations.pop_back();
        if(sets[currSet]->otherPoints.size() == numPerDay) {
            currSet++;
        }
    }

    // allow up to 100 swaps (if the radian split was not optimal)
    for(int k = 0; k < 100; k++){
        for(int i = 0; i < numDays; i++) {
            for(int j = i + 1; j < numDays; j++) {
                adjustSwaps(sets[i],sets[j]); //swap some of the elements that were not placed in the correct location
            }
        }
    }

    //set start and stops for each day
    for(int i = 0; i < numDays; i++) {
        sets[i]->setStart(starts.at(i));
        sets[i]->setStop(stops.at(i));
        sets[i]->travellingSalesman();
    }


}

void getAddressesFromFile(vector<LocationWidget *> &locations, string fileName, float minX,float maxX,float minY,float maxY,
                          float windowHeight, float windowWidth, dashBoard * dBoard, QWidget * window) {
    ifstream addressesFile (fileName);
    if (addressesFile.is_open()){
        string line;
        while ( getline (addressesFile,line) ) {

            float longitude = stof(line.substr(0,line.find(',')));
            float latitude = stof(line.substr(line.find(',') + 1,line.find('=')));
            QString address = QString::fromStdString(line.substr(line.find('=') + 1,line.length() - 1));

            latitude = (latitude - minX)/abs(maxX - minX)*windowWidth;
            longitude = (longitude - minY)/abs(maxY - minY)*windowHeight;
            LocationWidget * newLoc = new LocationWidget(window,latitude,longitude, windowWidth, windowHeight,dBoard, Qt::black);
            newLoc->setAddress(address);
            newLoc->setGeometry(0 + latitude - 6,0 - longitude - 6, 12, 12);
            locations.push_back(newLoc);
        }
        addressesFile.close();
    }

}

int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    QWidget window1;
    QWidget window2;
    int windowHeight = 682;
    int windowWidth = 1400;
    int dashBoardSize = 400;
    float maxX = -78.365; // maxLatitude (right of screen)
    float minX = -80.51; // minmum Latitude (left of screen)
    float maxY = 43.52; // max longtitude (top of screen)
    float minY = 44.275; // min longtitude (bottom of screen)
    int numDays = 5; //number of unqiue paths (days of work)
    int numPerDay = 6; //number of extra jobs per day
    QBrush pathColors [7] ={Qt::blue,Qt::yellow,Qt::darkCyan,Qt::gray,Qt::cyan,Qt::magenta,Qt::darkGreen};
    Path * sets [numDays]; // Path for each days work
    vector<LocationWidget *> locations; //additional locations to sort and find paths for
    vector<LocationWidget *> starts; // start locations for each day
    vector<LocationWidget *> stops; // end location for each day

    for(int i = 0; i < numDays; i++) {
        sets[i] = new Path(&window1,pathColors[i],true,windowWidth,windowHeight);
        sets[i]->setGeometry(0,0,windowWidth, windowHeight);
    }

    dashBoard * dBoard = new dashBoard(&window2,sets, numDays);
    dBoard->setGeometry(0,0, dashBoardSize, windowHeight);

    window1.setFixedSize(windowWidth, windowHeight);
    window2.setFixedSize(dashBoardSize, windowHeight);



    //get the long and lat for each address

    string command = "python addressConverter.py";
    system(command.c_str());

    getAddressesFromFile(locations, "addressesOut.txt",minX,maxX,minY,maxY,windowHeight,windowWidth,dBoard,&window1);
    getAddressesFromFile(starts, "startsOut.txt",minX,maxX,minY,maxY,windowHeight,windowWidth,dBoard,&window1);
    getAddressesFromFile(stops, "stopsOut.txt",minX,maxX,minY,maxY,windowHeight,windowWidth,dBoard,&window1);

    // make sure we have same amount of starts and stops
    if(starts.size() != stops.size()) {
        cerr << "not equal number of starts and stops" << endl;
        return -1;
    }
    numDays = starts.size();

    sort(locations.begin(),locations.end(),locationRadiansComparator);

    divideLocations(locations,starts,stops,sets, numDays, numPerDay);

    //set the color of the points
    for(int i = 0; i < numDays; i++) {
        for(int j = 0; j < sets[i]->otherPoints.size(); j++) {
            sets[i]->otherPoints.at(j)->setColor(pathColors[i]);
        }
    }


    //set background image
    QPixmap bkgnd("GTA.png");
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    window1.setPalette(palette);




    window1.show();
    window2.show();



    return app.exec();
}
