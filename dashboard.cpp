#include "dashboard.h"

dashBoard::dashBoard(QWidget *parent, Path * sets[], int n) : QWidget(parent),numDays(n)
{
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



    //mainLayout->addWidget(shapeLabel, 2, 0, Qt::AlignRight);
   // mainLayout->addWidget(shapeComboBox, 2, 1);


    //mainLayout->addWidget(transformationsCheckBox, 5, 2, 1, 2, Qt::AlignRight);
    setLayout(mainLayout);

}

void dashBoard::togglePath(int pathNum) {
    sets[pathNum]->setShowPath(setCheckBoxes[pathNum]->checkState());
}

void dashBoard::toggleLocation(int pathNum, int locationNum) {
    sets[pathNum]->otherPoints[locationNum]->highlight();
}
