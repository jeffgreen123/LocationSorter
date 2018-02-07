// basic file operations
#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>    
#include <vector>    
#include <string>  
using namespace std;

typedef std::pair<float,int> distancePathPair;
bool distancePairComparator ( const distancePathPair& l, const distancePathPair& r)
   { return l.first > r.first; }

struct Location {
    float latitude;
    float longtitude;
    vector<distancePathPair> distances; //max distance from all the days of the week paths
    float distanceBetweenNext;
    string address;
};

struct Path {
    Location start;
    Location stop;
    vector <Location> otherPoints;
};


typedef std::pair<float, Location> locationDistancePair;
bool locationPairCcomparator ( const locationDistancePair& l, const locationDistancePair& r)
   { return l.first < r.first; }


// Oshawa = 78.8658,43.8971

// Markham = 79.3370,43.8561

// Newmarket = 79.4613,44.0592

// Aurora = 79.4504,44.0065

// Toronto (Downtown) = 79.3832,43.6532

// Thornhill = 79.4591,43.8080

// Richmond Hill = 79.4403,43.8828

int numDays = 5; //number of unqiue paths (days of work)
int extra = 31; //number of outlying points
int numPerDay = 7; //number of extra jobs per day

// get the distance of a point to a given line (defined by (x1,y1) to (x2,y2))
float getLineDistance(float x, float y, float x1, float y1, float x2, float y2){
  float A = x - x1;
  float B = y - y1;
  float C = x2 - x1;
  float D = y2 - y1;

  float dot = A * C + B * D;
  float len_sq = C * C + D * D;
  float param = -1;
  if (len_sq != 0) //in case of 0 length line
      param = dot / len_sq;

  float xx, yy;

  if (param < 0) {
    xx = x1;
    yy = y1;
  }
  else if (param > 1) {
    xx = x2;
    yy = y2;
  }
  else {
    xx = x1 + param * C;
    yy = y1 + param * D;
  }

  float dx = x - xx;
  float dy = y - yy;
  return sqrtf(dx * dx + dy * dy);
}

//get the distance between the two best paths for the location
float getDistanceBetweenNext (Location l) {
    return l.distances.at(l.distances.size() - 2).first - l.distances.back().first;
}

int main (int argc, char *argv[]) {
    cout << argc << endl;
    //error checking
    if(extra >= numDays*numPerDay) {
        cerr << "NOT ENOUGH DAYS/JOBS PER DAY FOR THE NUMBER OF JOBS" << endl;
        cerr << "CURRENT MAX: " << numDays*numPerDay << endl;
        cerr << "NUM JOBS: " << extra <<endl;
        return -1;
    }
    //start locations
    Location s1 = {1,2};
    Location s2 = {2,2};
    Location s3 = {1,4};
    Location s4 = {1,1};
    Location s5 = {2,1};
    Location starts [] = {s1,s2,s3,s4,s5};

    //finish locations locations
    Location f1 = {-65,3};
    Location f2 = {-50,50};
    Location f3 = {3,56};
    Location f4 = {66,45};
    Location f5 = {56,3};

    Location finishes [] = {f1,f2,f3,f4,f5};

    //additional locations
    Location otherPoints [] = {{-52,1},{-45,1},{-44,3},{-38,4},{-22,2},{-15,3}, {-12,2},{-8,1}, {-6,4},
        {-49,51},{-48,52},{-46,54},{-27,51},
        {5,23},{4,33},{3,35},{4,44},{2,45},{3,55},
        {33,33},{45,45},{50,50},{52,52},{66,45},
        {35,3},{49,2},{50,3},{50,1},{52,2},{53,3},{54,1}};

    //create days (paths)
    Path path1 = {starts[0],finishes[0]};
    Path path2 = {starts[1],finishes[1]};
    Path path3 = {starts[2],finishes[2]};
    Path path4 = {starts[3],finishes[3]};
    Path path5 = {starts[4],finishes[4]};

    Path paths []   = {path1,path2,path3,path4,path5};

    // get closest distance for each extra point to each path
    for( int i = 0; i < numDays; i++) {

        float slope = (paths[i].stop.longtitude - paths[i].start.longtitude)/(paths[i].stop.latitude - paths[i].start.latitude);

        for(int j = 0; j < extra; j++){
            float distance = getLineDistance(otherPoints[j].latitude,otherPoints[j].longtitude,
                                                            paths[i].start.latitude,paths[i].start.longtitude,
                                                            paths[i].stop.latitude,paths[i].stop.longtitude);
            distancePathPair newPair = distancePathPair(distance,i);
            otherPoints[j].distances.push_back(newPair);
        }
        
    }

    vector <locationDistancePair> locationDistanceVector;
    for(int i = 0; i < extra; i++) {
        // sort the distances from least to greatest
        std::sort(otherPoints[i].distances.begin(),otherPoints[i].distances.begin()+numDays,distancePairComparator);
        // get the distance between the two best paths (the loss if we cant put it in the disired path)
        otherPoints[i].distanceBetweenNext = getDistanceBetweenNext(otherPoints[i]);

        locationDistanceVector.push_back(locationDistancePair(otherPoints[i].distanceBetweenNext,otherPoints[i]));
        //cout <<  otherPoints[i].distanceBetweenNext << endl;
    }

    std::sort(locationDistanceVector.begin(),locationDistanceVector.end(), locationPairCcomparator);
    for(int i = 0; i < extra; i++) {
        //cout <<  otherPoints[i].distanceBetweenNext << endl;
    }
    while(!locationDistanceVector.empty()) {
        // get the location with the largest difference in distances for paths (worst if cant fit in disired path)
        Location loc = locationDistanceVector.back().second;
        // get the desired path of this location
        int bestChoice = loc.distances.back().second;

        //cout << locationDistanceVector.size() << endl;
        // if can go in disired path, put it in 
        if(paths[bestChoice].otherPoints.size() < numPerDay) {
            // add location to path and remove location from pending locations
            paths[bestChoice].otherPoints.push_back(loc);
            locationDistanceVector.pop_back();
        }
        else {
            // pop off closest distance because path is full
            locationDistanceVector.back().second.distances.pop_back();
            // calculate new difference
            locationDistanceVector.back().second.distanceBetweenNext = getDistanceBetweenNext(locationDistanceVector.back().second);
            // resort the vector
            std::sort(locationDistanceVector.begin(),locationDistanceVector.end(), locationPairCcomparator);
        }
    }

    for(int i = 0; i < numDays; i++) {
        for(int j = 0; j < paths[i].otherPoints.size(); j++) {
            cout << paths[i].otherPoints.at(j).latitude << "," << paths[i].otherPoints[j].longtitude << ";";
        }
        cout << endl;
    }

  return 0;
}
