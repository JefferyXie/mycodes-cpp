TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# to support pthread
CONFIG += thread

# to support C++14
QMAKE_CXXFLAGS += -std=c++1y

#DEFINES += USE_SPINLOCK # this doesn't work?!
DEFINES += "USE_SPINLOCK=1"

DEFINES += "WINVER=0x0500" # test

SOURCES += main.cpp

HEADERS += \
    ../header.h \
    xfork.h \
    xposix.h \
    mutex_spinlock.h \
    memoryorder.h \
    join_detach.h

