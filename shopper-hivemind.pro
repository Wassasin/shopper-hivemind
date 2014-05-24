TEMPLATE = app
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_LFLAGS_RELEASE -= -O1

QMAKE_CXXFLAGS_DEBUG += -O0
QMAKE_CXXFLAGS_DEBUG += -fno-inline

CONFIG += debug_and_release

SOURCES += src/main.cpp \
	src/transaction.cpp \
    src/offer.cpp \
    src/history.cpp \
	src/typedefs.cpp \
	src/datarow.cpp \
	src/outputwriter.cpp \
    src/util/csvparser.cpp

linux: INCLUDEPATH += /usr/include/msgpack
INCLUDEPATH += /usr/include/qt5/QtCore
INCLUDEPATH += /usr/include/qt5

HEADERS += \
    src/transaction.h \
    src/typedefs.h \
	src/util/convertor.h \
	src/productmeasure.h \
	src/offer.h \
	src/history.h \
	src/datarow.h \
	src/outputwriter.h \
    src/util/msgpackwriter.h \
    src/util/msgpackreader.h \
    src/util/csvreader.h \
    src/util/csvparser.h

LIBS += \
  -lboost_date_time \
  -lboost_regex \
  -lboost_system \
  -lboost_iostreams \
  -lboost_filesystem \
  -lQt5Core \
  -lmsgpack
