#include "dashboard.h"

dashBoard::dashBoard(QWidget *parent, Path * sets[], int n) : QWidget(parent),numDays(n)
{
    currSelection = 0;
    select1 = nullptr;
    select2 = nullptr;
    QGridLayout *mainLayout = new QGridLayout;
    for(int i = 0; i <n; i++) {
        this->sets.push_back(sets[i]);
        QLabel * pathLabel = new QLabel(("Path" + QString::number(i)));
        QCheckBox * viewCheckBox = new QCheckBox();
        viewCheckBox->setCheckState(Qt::Checked);
        setCheckBoxes.push_back(viewCheckBox);
        setLabels.push_back(pathLabel);
        pathLabel->setBuddy(viewCheckBox);

        QSignalMapper mapper;

        connect(viewCheckBox, &QCheckBox::toggled, [=]{
            togglePath(i);
        });

        for(int j = 0; j < sets[i]->otherPoints.size();j++) {
            QPushButton * locationButton = new QPushButton(QString::number(sets[i]->otherPoints[j]->getX()) +
                                                           "," +
                                                           QString::number(sets[i]->otherPoints[j]->getY()));
            connect(locationButton, &QPushButton::pressed, [=]{
                toggleLocation(i,j);
            });
            mainLayout->addWidget(locationButton, j + 1, i*2, Qt::AlignTop);

        }

        mainLayout->addWidget(pathLabel, 0, i*2, Qt::AlignTop);
        mainLayout->addWidget(viewCheckBox, 0, i*2 + 1, Qt::AlignTop);
    }
    QLabel * selectionStaticLabel1 = new QLabel("Selection 1" );
    QLabel * selectionStaticLabel2 = new QLabel("Selection 2" );
    mainLayout->addWidget(selectionStaticLabel1,10,0,1,5,Qt::AlignLeft);
    mainLayout->addWidget(selectionStaticLabel2,10,5,1,5,Qt::AlignLeft);

    QLabel * coordinatesStaticLabel1 = new QLabel("Coordinates:" );
    QLabel * coordinatesStaticLabel2 = new QLabel("Coordinates:" );
    QLabel * coordinatesDynamicLabel1 = new QLabel();
    QLabel * coordinatesDynamicLabel2 = new QLabel();

    coordinatesLabels.push_back(coordinatesDynamicLabel1);
    coordinatesLabels.push_back(coordinatesDynamicLabel2);

    mainLayout->addWidget(coordinatesStaticLabel1,11,0,1,5,Qt::AlignLeft);
    mainLayout->addWidget(coordinatesStaticLabel2,11,5,1,5,Qt::AlignLeft);
    mainLayout->addWidget(coordinatesDynamicLabel1,12,0,1,5,Qt::AlignLeft);
    mainLayout->addWidget(coordinatesDynamicLabel2,12,5,1,5,Qt::AlignLeft);

    QLabel * addressStaticLabel1 = new QLabel("Address:" );
    QLabel * addressStaticLabel2 = new QLabel("Address:" );
    QLabel * addressDynamicLabel1 = new QLabel();
    QLabel * addressDynamicLabel2 = new QLabel();

    addressLabels.push_back(addressDynamicLabel1);
    addressLabels.push_back(addressDynamicLabel2);

    mainLayout->addWidget(addressStaticLabel1,13,0,1,5,Qt::AlignLeft);
    mainLayout->addWidget(addressStaticLabel2,13,5,1,5,Qt::AlignLeft);
    mainLayout->addWidget(addressDynamicLabel1,14,0,1,5,Qt::AlignLeft);
    mainLayout->addWidget(addressDynamicLabel2,14,5,1,5,Qt::AlignLeft);
    //mainLayout->addWidget(shapeLabel, 2, 0, Qt::AlignRight);
   // mainLayout->addWidget(shapeComboBox, 2, 1);

    QPushButton * swap = new QPushButton("Swap");

    connect(swap, SIGNAL(pressed()),this, SLOT(swapLocations()));
    mainLayout->addWidget(swap,15,5,1,5,Qt::AlignCenter);
    //mainLayout->addWidget(transformationsCheckBox, 5, 2, 1, 2, Qt::AlignRight);
    setLayout(mainLayout);

}
void dashBoard::swapLocations() {
        float tempX = select1->getX();
        float tempY = select1->getY();
        QBrush tempColor = select1->getColor();
        QString tempAddress = select1->getAddress();
        select1->setCoordinates(select2->getX(),select2->getY());
        select1->setColor(select2->getColor());
        select1->setAddress(select2->getAddress());
        select2->setCoordinates(tempX,tempY);
        select2->setColor(tempColor);
        select2->setAddress(tempAddress);
        for(int i = 0; i < sets.size(); i++) {
            sets.at(i)->travellingSalesman();
        }
    }

void dashBoard::togglePath(int pathNum) {
    sets[pathNum]->setShowPath(setCheckBoxes[pathNum]->checkState());
}

void dashBoard::toggleLocation(int pathNum, int locationNum) {
    LocationWidget * newLocation = sets[pathNum]->otherPoints[locationNum];
    if(newLocation != select1 && newLocation != select2) {
        coordinatesLabels.at(currSelection)->setText(QString::number(newLocation->getX()) +
                                                     "," +
                                                     QString::number(newLocation->getY()));
        addressLabels.at(currSelection)->setText("test");
        if(currSelection == 0){
            if(select1)
                select1->highlight();
            select1 = sets[pathNum]->otherPoints[locationNum];
            currSelection = 1;
            select1->highlight();
        }
        else {
            if(select2)
                select2->highlight();
            select2 = sets[pathNum]->otherPoints[locationNum];
            currSelection = 0;
            select2->highlight();
            }
        }
    }
