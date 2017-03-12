QT += core network
QT -= gui

CONFIG += c++11

TARGET = CR_server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    battle.cpp \
    unit.cpp \
    tower.cpp \
    minion.cpp

HEADERS += \
    battle.h \
    unit.h \
    tower.h \
    minion.h
