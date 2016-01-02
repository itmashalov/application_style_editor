QT += core gui svg xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = application_style_editor
TEMPLATE = app

CONFIG += mobility


#Include global defines
IS_SUB_MODULE=true
include(../application_style_editor.pro)

SOURCES += src/Editor.cpp
HEADERS += inc/general_application_style_editor/Editor.h


SOURCES += src/main.cpp

SOURCES += src/MainWindow.cpp
HEADERS += inc/general_application_style_editor/MainWindow.h



# Tdp libs
include(../tdp_qt_utils/dependencies.pri)
include(../htl_common_data_types/dependencies.pri)

include(../tdp_build/modules.pri)
include(../tdp_build/build.pri)

INCLUDEPATH  += ../general_application_style_editor/inc/



