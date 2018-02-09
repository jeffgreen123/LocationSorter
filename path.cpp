#include "path.h"

Path::Path(QWidget *parent) : QWidget(parent)
{

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

