QT -= gui
QT += network websockets sql

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        swiftapiclientkucoin.cpp \
        swiftapiparserkucoin.cpp

target.path = /opt/swift-bot/bin

HEADERS += \
    swiftapiclientkucoin.h \
    swiftapiparserkucoin.h

include( ../shared.pri )
