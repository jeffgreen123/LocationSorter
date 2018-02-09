#include <QApplication>
#include <QPushButton>
#include "circlewidget.h"
#include <vector>
#include <string>

using namespace std;
typedef std::pair<float,float> longLatPair;

typedef std::pair<float,int> distancePathPair;
bool distancePairComparator ( const distancePathPair& l, const distancePathPair& r)
   { return l.first > r.first; }


struct Location {
    float latitude;
    float longtitude;
    string address;
};

typedef std::pair<float, Location> locationRadiansPair;
bool locationRadiansComparator ( const locationRadiansPair& l, const locationRadiansPair& r)
   { return l.first > r.first; }

struct Path {
    Location start;
    Location stop;
    vector <locationRadiansPair> otherPoints;
};

int main(int argc, char **argv)
{
 QApplication app (argc, argv);
 int windowSize = 800;
 QWidget window;
 window.setFixedSize(windowSize, windowSize);
 vector<longLatPair> locations;
 locations.push_back(longLatPair(-52,1));
 locations.push_back(longLatPair(-45,1));
 locations.push_back(longLatPair(-44,3));
 locations.push_back(longLatPair(-38,4));
 locations.push_back(longLatPair(-22,2));
 locations.push_back(longLatPair(-15,3));
 locations.push_back(longLatPair(-12,2));
 locations.push_back(longLatPair(-8,1));
 locations.push_back(longLatPair(-6,4));
 locations.push_back(longLatPair(-49,51));
 locations.push_back(longLatPair(-48,52));
 locations.push_back(longLatPair(-46,54));
 locations.push_back(longLatPair(-27,51));
 locations.push_back(longLatPair(5,23));
 locations.push_back(longLatPair(4,33));
 locations.push_back(longLatPair(3,35));
 locations.push_back(longLatPair(4,44));
 locations.push_back(longLatPair(2,45));
 locations.push_back(longLatPair(3,55));
 locations.push_back(longLatPair(33,33));
 locations.push_back(longLatPair(45,45));
 locations.push_back(longLatPair(50,50));
 locations.push_back(longLatPair(52,52));
 locations.push_back(longLatPair(66,45));
 locations.push_back(longLatPair(35,3));
 locations.push_back(longLatPair(49,2));
 locations.push_back(longLatPair(50,3));
 locations.push_back(longLatPair(50,1));
 locations.push_back(longLatPair(52,2));
 locations.push_back(longLatPair(53,3));

 CircleWidget * points[30];

 for( int i = 0; i < 30; i++){
     points[i] = new CircleWidget(&window,4,4, windowSize);
     points[i]->setGeometry(400 + locations.at(i).first*4,400 - locations.at(i).second*4, 8, 8);
 }



 window.show();
 return app.exec();
}
