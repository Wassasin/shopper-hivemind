TEMPLATE = app
CONFIG += console
CONFIG += c++11
CONFIG += warn_on
CONFIG += debug_and_release

CONFIG -= app_bundle
CONFIG += qt
QT += core
QT -= opengl
QMAKE_CXXFLAGS_RELEASE += -O2
QMAKE_CXXFLAGS_RELEASE -= -O3
QMAKE_LFLAGS_RELEASE -= -O1

QMAKE_CXXFLAGS_DEBUG += -O0
QMAKE_CXXFLAGS_DEBUG += -fno-inline

QMAKE_CXXFLAGS_WARN_ON += -Wall
QMAKE_CXXFLAGS_WARN_ON += -Wextra
QMAKE_CXXFLAGS_WARN_ON += -Weffc++
QMAKE_CXXFLAGS_WARN_ON += -pedantic

SOURCES += src/main.cpp \
        src/data/transaction.cpp \
    src/data/offer.cpp \
    src/data/history.cpp \
        src/typedefs.cpp \
        src/data/datarow.cpp \
        src/outputwriter.cpp \
    src/util/csvparser.cpp \
    src/regressor.cpp \
    src/featureset.cpp \
    src/util/featureextractor.cpp

unix:!macx: QMAKE_CXXFLAGS += -isystem /usr/include/msgpack
unix:!macx: QMAKE_CXXFLAGS += -isystem /usr/include/qt5/QtCore
unix:!macx: QMAKE_CXXFLAGS += -isystem /usr/include/qt5
macx: INCLUDEPATH += /usr/local/include
INCLUDEPATH += src/ \
    /usr/include/libsvm \
    /usr/include/boost

HEADERS += \
    src/data/transaction.h \
    src/typedefs.h \
        src/data/productmeasure.h \
        src/data/offer.h \
        src/data/history.h \
        src/data/datarow.h \
        src/outputwriter.h \
    src/util/msgpackwriter.h \
    src/util/msgpackreader.h \
    src/util/csvreader.h \
    src/util/csvparser.h \
    src/data/client.h \
    src/cli.h \
    src/cache.h \
    src/util/reader.h \
    src/util/writer.h \
    src/regressor.h \
    src/featureset.h \
    src/util/featureextractor.h

unix:!macx: LIBS += \
  -lboost_date_time \
  -lboost_regex \
  -lboost_system \
  -lboost_iostreams \
  -lboost_filesystem \
  -lboost_program_options \
  -lQt5Core \
  -lmsgpack \
  -lsvm

macx: LIBS += \
  -L/usr/local/lib -lboost_date_time \
  -lboost_regex \
  -lboost_system \
  -lboost_iostreams \
  -lboost_filesystem \
  -lboost_program_options \
  -lmsgpack \
  -lsvm
