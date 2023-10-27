QT += core gui widgets

TARGET = Honor8
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    main.cpp \
    datasrc.cpp \
    chart.cpp \
    mwidget.cpp \
    cwidget.cpp \
    dialog.cpp \
    timeaxis.cpp \
    valueaxis.cpp

HEADERS += \
    util.h \
    datasrc.h \
    typedef.h \
    chart.h \
    config.h \
    cwidget.h \
    mwidget.h \
    dialog.h \
    timeaxis.h \
    struct.h \
    valueaxis.h
