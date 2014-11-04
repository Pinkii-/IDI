TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lglut -lGL -lGLU -pthread

SOURCES += main.cpp \
    model/model.cpp

HEADERS += \
    model/model.h
