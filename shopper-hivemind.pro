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
    src/util/csv_parser.cpp \
	src/transaction.cpp \
    src/offer.cpp \
    src/history.cpp \
	src/typedefs.cpp \
	src/datarow.cpp \
	src/outputwriter.cpp

linux: INCLUDEPATH += /usr/include/msgpack
INCLUDEPATH += /usr/include/qt5/QtCore
INCLUDEPATH += /usr/include/qt5

HEADERS += \
    src/util/csv_parser.h \
    src/transaction.h \
    src/typedefs.h \
	src/util/convertor.h \
	src/util/csv_reader.h \
	src/util/msgpack_writer.h \
	src/util/msgpack_reader.h \
	src/productmeasure.h \
	src/offer.h \
	src/history.h \
	src/datarow.h \
	src/outputwriter.h

LIBS += \
  -lboost_date_time \
  -lboost_regex \
  -lboost_system \
  -lboost_iostreams \
  -lboost_filesystem \
  -lQt5Core \
  -lmsgpack
