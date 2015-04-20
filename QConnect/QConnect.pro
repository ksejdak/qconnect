#-------------------------------------------------
#
# Project created by QtCreator 2015-04-18T12:35:08
#
#-------------------------------------------------

QT       += core gui network

RC_FILE  = QConnect.rc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QConnect
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    TCPServer.cpp \
    TCPClient.cpp

HEADERS  += MainWindow.h \
    TCPServer.h \
    TCPClient.h \
    Version.h

FORMS    += MainWindow.ui \
    TCPServer.ui \
    TCPClient.ui

RESOURCES += resources.qrc
