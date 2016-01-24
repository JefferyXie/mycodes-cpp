TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# to support multi-thread
CONFIG += thread
# http://stackoverflow.com/questions/718447/adding-external-library-into-qt-creator-project
#LIBS += -lpthread # also works as above line

# to support C++11, will give warning with C++14 line
#CONFIG += C++11
#CONFIG += C++14

# to support C++14
QMAKE_CXXFLAGS += -std=c++1y

SOURCES += main.cpp

HEADERS += \
    ../header.h \
    ../utility.h \
    ../language/stlClass.h \
    ../language/sizeofClass.h \
    ../language/MyList.h \
    ../language/MyDictionary.h \
    ../language/C++11.h \
    ../algorithm/sherlock.h

OTHER_FILES += \
    ../Python/socket_client.py \
    ../Python/socket_server.py


