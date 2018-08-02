#-------------------------------------------------
#
# Project created by QtCreator 2017-10-06T21:34:38
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#TARGET = openserial

target.path = /home/pi/
INSTALLS += target

LIBS+=-lwiringPi

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
        main.cpp \
        mainwindow.cpp \
    nature.cpp \
    resistance.cpp \
    four_electrodes.cpp \
    serial_config.cpp \
    exploit.cpp \
    keyboard.cpp \
    Sendhread.cpp \
    about.cpp \
    power.cpp \
    qcustomplot.cpp \
    device_co.cpp

HEADERS += \
        mainwindow.h \
    nature.h \
    resistance.h \
    four_electrodes.h \
    serial_config.h \
    exploit.h \
    keyboard.h \
    Sendthread.h \
    about.h \
    power.h \
    qcustomplot.h \
    device_co.h

FORMS += \
        mainwindow.ui \
    dialog.ui \
    nature.ui \
    resistance.ui \
    four_electrodes.ui \
    exploit.ui \
    keyboard.ui \
    power.ui \
    qcustomplot.ui \
    device_co.ui

RESOURCES += \
    ../Demo/res.qrc

