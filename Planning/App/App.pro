#-------------------------------------------------
#
# Project created by QtCreator 2018-02-28T16:14:53
#
#-------------------------------------------------

QT       += core gui
QT       += charts
QT       += sql
CONFIG   += c++14
QMAKE_CXXFLAGS += -std=c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Planning
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += .
DEPENDPATH += .

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    WorkSpace/WorkSpaceWidget.cpp \
    WorkSpace/WorkSpaceController.cpp \
    Waiting/BusyThread.cpp \
    Waiting/BusyIndicator.cpp \
    Waiting/BusyDialog.cpp \
    Settings/SettingsDialog.cpp \
    Settings/ISettings.cpp \
    Settings/DbSettingsWidget.cpp \
    PlanningTree/PlanningWidget.cpp \
    DataReading/MappingWidget.cpp \
    DataReading/DataHandler.cpp \
    DataReading/CheckBoxItemDelegate.cpp \
    Charts/WorkLoadSeries.cpp \
    Charts/WorkLoadChartWidget.cpp \
    Charts/WorkLoadChart.cpp \
    Charts/PlanningSeries.cpp \
    Charts/PlanningChartWindow.cpp \
    Charts/PlanningChartWidget.cpp \
    Charts/PlanningChart.cpp \
    Charts/IChart.cpp \
    Charts/BaseChartWindow.cpp \
    Charts/BaseChartWidget.cpp \
    DbManager.cpp \
    ChartsWidget.cpp \
    HelpDialog.cpp \
    main.cpp \
    MainWindow.cpp \
    PlanningTree/FilterComboBox.cpp \
    WorkSpace/WorkSpaceView.cpp

HEADERS += \
    WorkSpace/WorkSpaceWidget.h \
    WorkSpace/WorkSpaceController.h \
    Waiting/BusyThread.h \
    Waiting/BusyManager.h \
    Waiting/BusyIndicator.h \
    Waiting/BusyDialog.h \
    Settings/SettingsDialog.h \
    Settings/ISettings.h \
    Settings/DbSettingsWidget.h \
    PlanningTree/PlanningWidget.h \
    DataReading/MappingWidget.h \
    DataReading/DataHandler.h \
    DataReading/CheckBoxItemDelegate.h \
    Charts/WorkLoadSeries.h \
    Charts/WorkLoadChartWidget.h \
    Charts/WorkLoadChart.h \
    Charts/PlanningSeries.h \
    Charts/PlanningChartWindow.h \
    Charts/PlanningChartWidget.h \
    Charts/PlanningChart.h \
    Charts/IChart.h \
    Charts/BaseChartWindow.h \
    Charts/BaseChartWidget.h \
    MainWindow.h \
    HelpDialog.h \
    Globals.h \
    DbManager.h \
    ChartsWidget.h \
    PlanningTree/FilterComboBox.h \
    WorkSpace/WorkSpaceView.h

FORMS += \
    WorkSpace/WorkSpaceWidget.ui \
    Waiting/BusyDialog.ui \
    Settings/SettingsDialog.ui \
    Settings/DbSettingsWidget.ui \
    PlanningTree/PlanningWidget.ui \
    DataReading/MappingWidget.ui \
    Charts/BaseChartWindow.ui \
    Charts/BaseChartWidget.ui \
    MainWindow.ui \
    HelpDialog.ui \
    ChartsWidget.ui

RESOURCES += \
    resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Engine/release/ -lEngine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Engine/debug/ -lEngine
else:unix: LIBS += -L$$OUT_PWD/../Engine/ -lEngine

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Data/release/ -lData
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Data/debug/ -lData
else:unix: LIBS += -L$$OUT_PWD/../Data/ -lData

INCLUDEPATH += $$PWD/../Engine
DEPENDPATH += $$PWD/../Engine
INCLUDEPATH += $$PWD/../Data
DEPENDPATH += $$PWD/../Data

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Engine/release/Engine.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Engine/debug/Engine.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../Engine/libEngine.a

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Data/release/Data.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Data/debug/Data.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../Data/libData.a


