TEMPLATE = app
TARGET = LocationSorter

QT = core gui
QT += positioning
QT += location
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
SOURCES += \
    main.cpp \
    locationwidget.cpp \
    path.cpp \
    dashboard.cpp \
    helperfunctions.cpp

HEADERS += \
    locationwidget.h \
    path.h \
    dashboard.h \
    helperfunctions.h
