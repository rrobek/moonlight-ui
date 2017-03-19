#-------------------------------------------------
#
# Project created by QtCreator 2017-03-19T16:58:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = moonlight-ui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    moonlighttask.cpp \
    optionsdialog.cpp

HEADERS  += mainwindow.h \
    moonlighttask.h \
    optionsdialog.h

FORMS    += mainwindow.ui \
    optionsdialog.ui
