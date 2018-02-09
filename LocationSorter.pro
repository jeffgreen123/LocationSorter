TEMPLATE = app
TARGET = LocationSorter

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
SOURCES += \
    main.cpp \
    locationwidget.cpp \
    path.cpp

HEADERS += \
    locationwidget.h \
    path.h
