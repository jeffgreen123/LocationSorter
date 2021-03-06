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
#include <QGeoCodingManager>
#include <QGeoServiceProvider>
#include <limits.h>
#include <unistd.h>
using namespace std;







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

    //set start and stops for each day
    for(int i = 0; i < numDays; i++) {
        sets[i]->setStart(starts.at(i));
        sets[i]->setStop(stops.at(i));
        sets[i]->travellingSalesman();
    }
}


//get addresses for starts and stops from file
void getAddressesFromFile(vector<LocationWidget *> &locations, string fileName, float minX,float maxX,float minY,float maxY,
                          float windowHeight, float windowWidth, dashBoard * dBoard, QWidget * window) {
    /*file looks like
     * long,lat=address
     * long,lat=address
     *
     * long,lat=address
     * long,lat=address
    */
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

//get addresses for main sets from file
void getAddressesFromFile(Path * sets[], string fileName, float minX,float maxX,float minY,float maxY,
                          float windowHeight, float windowWidth, dashBoard * dBoard, QWidget * window) {
    /*file looks like
     * long,lat=address
     * long,lat=address
     *
     * long,lat=address
     * long,lat=address
    */
    ifstream addressesFile (fileName);
    if (addressesFile.is_open()){
        string line;
        int currSet = 0;
        while ( getline (addressesFile,line) ) {
            if(line == ""){
                currSet++;
            }
            else{
                float longitude = stof(line.substr(0,line.find(',')));
                float latitude = stof(line.substr(line.find(',') + 1,line.find('=')));
                QString address = QString::fromStdString(line.substr(line.find('=') + 1,line.length() - 1));

                latitude = (latitude - minX)/abs(maxX - minX)*windowWidth;
                longitude = (longitude - minY)/abs(maxY - minY)*windowHeight;
                LocationWidget * newLoc = new LocationWidget(window,latitude,longitude, windowWidth, windowHeight,dBoard, Qt::black);
                newLoc->setAddress(address);
                newLoc->setGeometry(0 + latitude - 6,0 - longitude - 6, 12, 12);
                sets[currSet]->otherPoints.push_back(newLoc);
            }
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
    int numPerDay = 7; //number of extra jobs per day
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

    getAddressesFromFile(sets, "addressesOut.txt",minX,maxX,minY,maxY,windowHeight,windowWidth,dBoard,&window1);
    getAddressesFromFile(starts, "startsOut.txt",minX,maxX,minY,maxY,windowHeight,windowWidth,dBoard,&window1);
    getAddressesFromFile(stops, "stopsOut.txt",minX,maxX,minY,maxY,windowHeight,windowWidth,dBoard,&window1);

    // make sure we have same amount of starts and stops
    if(starts.size() != stops.size()) {
        cerr << "not equal number of starts and stops" << endl;
        return -1;
    }

    numDays = starts.size();

    divideLocations(locations,starts,stops,sets, numDays, numPerDay);

    //set the color of the points
    for(int i = 0; i < numDays; i++) {
        for(int j = 0; j < sets[i]->otherPoints.size(); j++) {
            sets[i]->otherPoints.at(j)->setColor(pathColors[i]);
        }
    }

    for(int i = 0; i < numDays; i++) {
        starts[i]->setColor(Qt::green);
        stops[i]->setColor(Qt::red);
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
