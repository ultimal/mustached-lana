#-------------------------------------------------
#
# Project created by QtCreator 2012-04-25T14:57:18
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = mustached-lana
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    server.cpp \
    connection.cpp \
    datastore.cpp \
    nodemanager.cpp \
    node.cpp \
    nodeserver.cpp \
    nodeconnection.cpp

HEADERS += \
    server.h \
    connection.h \
    datastore.h \
    nodemanager.h \
    node.h \
    nodeserver.h \
    nodeconnection.h
