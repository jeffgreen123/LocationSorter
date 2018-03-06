#include "path.h"

using namespace std;
Path::Path(QWidget *parent, QBrush color, bool bShowPath, int windowWidth, int windowHeight) : QWidget(parent), color(color), bShowPath(bShowPath)
    ,windowHeight(windowHeight),windowWidth(windowWidth) {

}

LocationWidget* Path::getStart(){
    return start;
}

void Path::setStart(LocationWidget * l){
    start = l;
}

LocationWidget* Path::getStop(){
    return stop;
}

void Path::setStop(LocationWidget * l){
    stop = l;
}

void Path::setColor(QBrush newColor) {
    color = newColor;
    repaint();
}

void Path::setShowPath(bool newShowPath) {
    bShowPath = newShowPath;
    repaint();
}

void Path::travellingSalesman() {
    //cout << numJobs <<endl;
    int numJobs = otherPoints.size();
    float minDistance = 1000000000;
    vector<LocationWidget*> finalPath;
    int pathIter[numJobs];
    int bestPath[numJobs];
    for(int i = 0; i < numJobs; i++) {
        pathIter[i] = i;
    }
    if(numJobs != 0){
        do {

            float distance = getDistanceBetweenPoints(getStart(),otherPoints.at(pathIter[0])) +
                getDistanceBetweenPoints(getStop(),otherPoints.at(pathIter[numJobs - 1]));
            for(int i = 0; i < numJobs - 1; i++) {
                distance += getDistanceBetweenPoints(otherPoints.at(pathIter[i]),otherPoints.at(pathIter[i+1]));
            }
            if(distance <= minDistance){
                for(int i = 0; i < numJobs; i++) {
                    bestPath[i] = pathIter[i];
                }
                    minDistance = distance;
            }
      } while ( std::next_permutation(pathIter, pathIter + numJobs));
      for(int i = 0; i < numJobs; i++) {
           finalPath.push_back(otherPoints.at(bestPath[i]));
      }
      otherPoints = finalPath;
    }
  cout << "DISTANCE:"  << minDistance << endl;
  cout << getStart()->getX() << ',' << getStart()->getY() << " to ";

  for( int i = 0; i < numJobs; i++) {
      cout << finalPath.at(i)->getX() << ',' << finalPath.at(i)->getY() << " to ";
  }
  repaint();
}

void Path::paintEvent(QPaintEvent *event){
    if(bShowPath && otherPoints.size() > 0) {
        QPainter painter(this);
        painter.setPen(QPen(color, 3, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(start->getX() ,0 - start->getY() , otherPoints.at(0)->getX() ,0 - otherPoints.at(0)->getY() );
        for(int i = 0; i < otherPoints.size() - 1; i++) {
            painter.drawLine(otherPoints.at(i)->getX(),0 - otherPoints.at(i)->getY(), otherPoints.at(i+1)->getX(),0 - otherPoints.at(i+1)->getY());
        }
        painter.drawLine(stop->getX(),0 - stop->getY(),otherPoints.at(otherPoints.size() - 1)->getX(),0 - otherPoints.at(otherPoints.size() - 1)->getY());
    }

}
