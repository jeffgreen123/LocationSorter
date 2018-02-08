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

typedef std::pair<float, Location> locationRadiansPair;
bool locationRadiansComparator ( const locationRadiansPair& l, const locationRadiansPair& r)
   { return l.first > r.first; }

struct Path {
    Location start;
    Location stop;
    vector <locationRadiansPair> otherPoints;
};

/*
typedef std::pair<float, Location> locationDistancePair;
bool locationPairCcomparator ( const locationDistancePair& l, const locationDistancePair& r)
   { return l.first < r.first; }
*/




// Oshawa = 78.8658,43.8971

// Markham = 79.3370,43.8561

// Newmarket = 79.4613,44.0592

// Aurora = 79.4504,44.0065

// Toronto (Downtown) = 79.3832,43.6532

// Thornhill = 79.4591,43.8080

// Richmond Hill = 79.4403,43.8828

int numDays = 5; //number of unqiue paths (days of work)
int extra = 30; //number of outlying points
int numPerDay = 7; //number of extra jobs per day

float getAverageRadians(vector<locationRadiansPair> locations) {
    float average = 0;
    for(int i = 0; i < locations.size(); i++) {
        average += locations.at(i).first;
    }
    average = average/locations.size();
    return average;
}

float getDistanceBetweenPoints(Location l1, Location l2) {
    float distX = fabs(l1.latitude - l2.latitude);
    float distY = fabs(l1.longtitude - l2.longtitude);
    //cout << l1.latitude << ',' <<  l1.longtitude <<  endl;
    return sqrtf(pow(distX,2) + pow(distY,2));
}

float getAverageDistance(vector<locationRadiansPair> locations, Location point, int currIndex) {
    float average = 0;
    for(int i = 0; i < locations.size(); i++) {
        if( i != currIndex) {
            average += getDistanceBetweenPoints(locations.at(i).second, point);
        }
    }
    average = average/(locations.size() - 1);
    return average;
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
        
        float distance = getDistanceBetweenPoints(set.start,set.otherPoints.at(order[0]).second) + 
            getDistanceBetweenPoints(set.stop,set.otherPoints.at(order[numJobs-1]).second);
        for(int i = 0; i < numJobs - 1; i++) {
            distance += getDistanceBetweenPoints(set.otherPoints.at(order[i]).second,set.otherPoints.at(order[i + 1]).second);
        }
        if(distance <= minDistance){
            for(int i = 0; i < numJobs; i++){
                finalPath[i] = order[i];
            }
            minDistance = distance;
        }
  } while ( std::next_permutation(order, order + numJobs) );

  cout << "DISTANCE:"  << minDistance << endl;
  cout << set.start.latitude << ',' << set.start.longtitude << " to ";

  for( int i = 0; i < numJobs; i++) {
      cout << set.otherPoints.at(finalPath[i]).second.latitude << ',' << set.otherPoints.at(finalPath[i]).second.longtitude << " to ";
  }

  cout << set.stop.latitude << ',' << set.stop.longtitude << endl;
}

void adjustUnevenDays(Path sets[]) {
    float averages[numDays];
    for(int i = 0; i < numDays; i++) {
        averages[i] = getAverageRadians(sets[i].otherPoints);
    }
    for(int i = numDays - 2; i > 0; i--) {
        for( int j = 0; j < sets[i].otherPoints.size(); j++) {
            //cout << i << endl;
            if(fabs(sets[i].otherPoints.at(j).first - averages[i + 1]) <= fabs(sets[i].otherPoints.at(j).first - averages[i]) 
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
            float averageL2P1Dist = getAverageDistance(p1.otherPoints, p2.otherPoints.at(j).second,j);
            float averageL2P2Dist = getAverageDistance(p2.otherPoints, p2.otherPoints.at(j).second,j);
            float averageL1P1Dist = getAverageDistance(p1.otherPoints, p1.otherPoints.at(i).second,i);
            float averageL1P2Dist = getAverageDistance(p2.otherPoints, p1.otherPoints.at(i).second,i);
            
            if((averageL1P1Dist - averageL1P2Dist) + (averageL2P2Dist - averageL2P1Dist) >= 0) {
                cout << "SWAP" << endl;
                cout << p1.otherPoints.at(i).second.latitude << "," << p1.otherPoints.at(i).second.longtitude << endl;
                cout << "AND" << endl;
                cout << p2.otherPoints.at(j).second.latitude << "," << p2.otherPoints.at(j).second.longtitude << endl;
                locationRadiansPair temp = p2.otherPoints.at(j);
                p2.otherPoints.at(j) = p1.otherPoints.at(i);
                p1.otherPoints.at(i) = temp;
            }
        }
     }
}

void divideLocations(vector <locationRadiansPair> locations) {
    Path sets [numDays];
    Location s1 = {1,2};
    Location f1 = {5,3};

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
        sets[i].start = s1;
        sets[i].stop = f1;
        travellingSalesman(sets[i],sets[i].otherPoints.size());
    }

    for(int i = 0; i < numDays; i++) {
        cout << "PATH " << i << endl;
        for(int j = 0; j < sets[i].otherPoints.size(); j++) {
            cout << sets[i].otherPoints.at(j).second.latitude << "," << sets[i].otherPoints.at(j).second.longtitude << " " ;
        }
        cout << endl;
    }
}


int main (int argc, char *argv[]) {
    cout << argc << endl;
    //error checking
    if(extra > numDays*numPerDay) {
        cerr << "NOT ENOUGH DAYS/JOBS PER DAY FOR THE NUMBER OF JOBS" << endl;
        cerr << "CURRENT MAX: " << numDays*numPerDay << endl;
        cerr << "NUM JOBS: " << extra <<endl;
        return -1;
    }
    //start locations
    Location s1 = {1,2};
    Location s2 = {1,2};
    Location s3 = {1,2};
    Location s4 = {1,2};
    Location s5 = {1,2};
    Location starts [] = {s1,s2,s3,s4,s5};

    //finish locations locations
    Location f1 = {5,3};
    Location f2 = {5,3};
    Location f3 = {5,3};
    Location f4 = {5,3};
    Location f5 = {5,3};

    Location finishes [] = {f1,f2,f3,f4,f5};

    //additional locations

    Location otherPoints [] = {{-52,1},{-45,1},{-44,3},{-38,4},{-22,2},{-15,3}, {-12,2},{-8,1}, {-6,4},
        {-49,51},{-48,52},{-46,54},{-27,51},
        {5,23},{4,33},{3,35},{4,44},{2,45},{3,55},
        {33,33},{45,45},{50,50},{52,52},{66,45},
        {35,3},{49,2},{50,3},{50,1},{52,2},{53,3}};
    vector<locationRadiansPair> locationsR;
        for( int i = 0; i < extra; i++){
            locationRadiansPair pair = locationRadiansPair(atan2(otherPoints[i].longtitude,otherPoints[i].latitude),otherPoints[i]);
            locationsR.push_back(pair);
        }
    vector <Location> otherPointsVec (otherPoints , otherPoints  + sizeof(otherPoints ) / sizeof(otherPoints [0])); 

    //create days (paths)
    Path path1 = {starts[0],finishes[0]};
    Path path2 = {starts[1],finishes[1]};
    Path path3 = {starts[2],finishes[2]};
    Path path4 = {starts[3],finishes[3]};
    Path path5 = {starts[4],finishes[4]};

    Path paths []   = {path1,path2,path3,path4,path5};
    sort(locationsR.begin(),locationsR.end(),locationRadiansComparator);
    
    divideLocations(locationsR);

  return 0;
}
