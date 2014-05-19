#-------------------------------------------------
#
# Project created by QtCreator 2014-05-19T11:35:41
#
#-------------------------------------------------
CONFIG += c++11
cache();
QT       += core
QT       -= gui

TARGET = shopper-hivemind
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    outputwriter.cpp \
    datarow.cpp

HEADERS += \
    outputwriter.h \
    datarow.h \
    constants.h
