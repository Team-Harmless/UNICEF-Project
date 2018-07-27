#-------------------------------------------------
#
# Project created by QtCreator 2018-07-13T13:52:58
#
#-------------------------------------------------

QT       += core gui
QT       += positioning
QT += network
QT += quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UNICEF-Project
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp\
        mainwindow.cpp \
        extractdata.cpp \
        place.cpp \
        school.cpp \
        healthfacility.cpp \
        search.cpp \
        comparisons.cpp \
        quadTree.cpp \
        context.cpp \
    browserintegrator.cpp
HEADERS += \
        mainwindow.h \
        extractdata.h \
        place.h \
        school.h \
        healthfacility.h \
        search.h \
        comparisons.h \
        quadtree.h \
        context.h \
    browserintegrator.h
FORMS += \
        mainwindow.ui

QMAKE_CXXFLAGS += -std=c++0x

DISTFILES += \
    Map.qml \
    Sprite.qml \
    Scripts.js \
    Lines.qml

#copydata.commands  = $(COPY_DIR) $$PWD/*.qml $$OUT_PWD; $(COPY_DIR) $$PWD/*.js $$OUT_PWD
#first.depends = $(first) copydata
#export(first.depends)
#export(copydata.commands)
#QMAKE_EXTRA_TARGETS += first copydata
