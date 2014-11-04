TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -fpermissive -Wwrite-strings

LIBS += -lglut -lGL -pthread

SOURCES += main.cpp \
    model/model.cpp

HEADERS += \
    model/model.h
