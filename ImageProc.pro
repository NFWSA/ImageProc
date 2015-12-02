#-------------------------------------------------
#
# Project created by QtCreator 2015-11-29T16:53:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = ImageProc
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    grayinfowidget.cpp \
    imagewidget.cpp \
    imageproc.cpp \
    bitplanedialog.cpp \
    spacealter.cpp \
    convolutiondialog.cpp

HEADERS  += mainwindow.h \
    grayinfowidget.h \
    imagewidget.h \
    imageproc.h \
    bitplanedialog.h \
    spacealter.h \
    convolutiondialog.h

FORMS += \
    grayinfowidget.ui \
    imagewidget.ui \
    imageproc.ui \
    bitplanedialog.ui \
    spacealter.ui \
    convolutiondialog.ui

DISTFILES += \
    .gitignore
