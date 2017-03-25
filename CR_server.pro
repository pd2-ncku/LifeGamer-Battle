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
    minion.cpp \
    humanknight.cpp \
    humanpriest.cpp \
    humanthief.cpp \
    elfgiant.cpp \
    elfwisp.cpp \
    elfarcher.cpp \
    undeadsamurai.cpp \
    sgram.cpp \
    player.cpp \
    rendercommunicator.cpp

HEADERS += \
    battle.h \
    unit.h \
    tower.h \
    minion.h \
    humanknight.h \
    humanpriest.h \
    humanthief.h \
    elfgiant.h \
    elfwisp.h \
    elfarcher.h \
    undeadsamurai.h \
    sgram.h \
    player.h \
    rendercommunicator.h
