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
    vector<LocationWidget*> finalPath;

    float minDistance = 1000000000;
    do {

        float distance = getDistanceBetweenPoints(getStart(),otherPoints.at(0)) +
            getDistanceBetweenPoints(getStop(),otherPoints.at(numJobs-1));
        for(int i = 0; i < numJobs - 1; i++) {
            distance += getDistanceBetweenPoints(otherPoints.at(i),otherPoints.at(i+1));
        }
        if(distance <= minDistance){
            for(int i = 0; i < numJobs; i++){
                finalPath = otherPoints;
            }
            minDistance = distance;
        }
  } while ( std::next_permutation(otherPoints.begin(), otherPoints.end()) );
  otherPoints = finalPath;
  //cout << "DISTANCE:"  << minDistance << endl;
  //cout << getStart()->getX() << ',' << getStart()->getY() << " to ";

  for( int i = 0; i < numJobs; i++) {
      //cout << finalPath.at(i)->getX() << ',' << finalPath.at(i)->getY() << " to ";
  }
  repaint();
  //cout << getStop()->getX() << ',' << getStop()->getY() << endl;
}

void Path::mousePressEvent(QMouseEvent* event){
    bShowPath = false;
    repaint();
    //QGraphicsView::mousePressEvent(event);
}
void Path::paintEvent(QPaintEvent *event){
    if(bShowPath) {
        QPainter painter(this);
        painter.setPen(QPen(color, 3, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(windowWidth/2 + start->getX()*4 + 50,windowHeight/2 - start->getY()*4 + 50,windowWidth/2 + otherPoints.at(0)->getX()*4 + 50,windowHeight/2 - otherPoints.at(0)->getY()*4 + 50);
        for(int i = 0; i < otherPoints.size() - 1; i++) {
            painter.drawLine(windowWidth/2 + otherPoints.at(i)->getX()*4 + 50,windowHeight/2 - otherPoints.at(i)->getY()*4 + 50,windowWidth/2 + otherPoints.at(i+1)->getX()*4 + 50,windowHeight/2 - otherPoints.at(i+1)->getY()*4 + 50);
        }
        painter.drawLine(windowWidth/2 + stop->getX()*4 + 50,windowHeight/2 - stop->getY()*4 + 50,windowWidth/2 + otherPoints.at(otherPoints.size() - 1)->getX()*4 + 50,windowHeight/2 - otherPoints.at(otherPoints.size() - 1)->getY()*4 + 50);
    }

}
