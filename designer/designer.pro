#-------------------------------------------------
#
# Project created by QtCreator 2018-09-17T09:44:26
#
#-------------------------------------------------

QT += designer
CONFIG += plugin

TARGET = $$qtLibraryTarget(NumericTable)
TEMPLATE = lib
DESTDIR = $$PWD/../lib

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS += target

DEFINES += NUMERICTABLE_LIB
DEFINES += QT_DEPRECATED_WARNINGS

include($$PWD/../src/NumericTable.pri)

HEADERS += \
    NumericTablePlugin.h

SOURCES += \
    NumericTablePlugin.cpp
