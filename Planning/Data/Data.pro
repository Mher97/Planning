#-------------------------------------------------
#
# Project created by QtCreator 2018-08-18T12:06:16
#
#-------------------------------------------------

QT       += gui

TARGET = Data
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++14
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BaseModel.cpp \
    BaseItem.cpp \
    BaseProxyModel.cpp \
    WorkSpaceItem.cpp \
    BranchItem.cpp \
    ProjectItem.cpp \
    MonitorItem.cpp \
    PlanningProxyModel.cpp \
    PlanningModel.cpp \
    PlanningItem.cpp \
    MappingProxyModel.cpp \
    MappingModel.cpp \
    MappingItem.cpp \
    FilterModel.cpp \
    FilterItem.cpp \
    WorkSpaceProxyModel.cpp \
    WorkSpaceModel.cpp

HEADERS += \
    BaseModel.h \
    ItemType.h \
    BaseItem.h \
    BaseProxyModel.h \
    WorkSpaceItem.h \
    BranchItem.h \
    ProjectItem.h \
    MonitorItem.h \
    PlanningProxyModel.h \
    PlanningModel.h \
    PlanningItem.h \
    MappingProxyModel.h \
    MappingModel.h \
    MappingItem.h \
    FilterModel.h \
    FilterItem.h \
    WorkSpaceProxyModel.h \
    WorkSpaceModel.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
