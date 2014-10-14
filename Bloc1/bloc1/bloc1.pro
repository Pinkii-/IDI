TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -fpermissive

LIBS += -lglut -lGL

SOURCES += main.cpp
