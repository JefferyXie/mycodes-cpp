TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# to support C++14
QMAKE_CXXFLAGS += -std=c++1y

SOURCES += main.cpp \
    xsocketserver.cpp \
    xsocketclient.cpp

HEADERS += \
    ../header.h \
    xsocketserver.h \
    xsocketclient.h

