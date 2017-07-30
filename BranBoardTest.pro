#-------------------------------------------------
#
# Project created by QtCreator 2017-03-17T15:59:02
#
#-------------------------------------------------

QT       += core gui network serialport

RC_ICONS += resource/images/logo/logo.ico
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BranBoardTest
TEMPLATE = app

#DEFINES += TEST_MODE
DEFINES -= UNICODE
DEFINES +=UMBCS

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    server/serverUser.cpp \
    server/networkManager.cpp \
    server/serverMIIO.cpp \
    server/serverSync.cpp \
    server/serverHodor.cpp \
    hodor/hodorControl.cpp \
    board/testItem/baseTestItem.cpp \
    board/testItem/testCPU.cpp \
    board/testItem/testGravity.cpp \
    board/testItem/testMIIO.cpp \
    board/testItem/testRTC.cpp \
    board/testItem/testUSB.cpp \
    board/testItem/testVOL.cpp \
    board/testItem/testWiFi.cpp \
    control/sysData.cpp \
    widget/widgetMain.cpp \
    widget/hodor/widgetHodor.cpp \
    widget/board/widgetControl.cpp \
    widget/board/widgetSetting.cpp \
    widget/board/base/statusMore.cpp \
    widget/board/base/statusTest.cpp \
    widget/board/base/widgetDebugInfo.cpp \
    widget/board/item/widgetTest.cpp \
    file/fileConfig.cpp \
    file/fileHodor.cpp \
    control/DeviceControl.cpp \
    control/SerialControl.cpp \
    widget/board/item/widgetItem.cpp \
    board/serialItem.cpp \
    board/deviceItem.cpp \
    board/testControl.cpp \
    board/testItem/testSYNC.cpp

HEADERS  += server/serverUser.h \
    server/networkManager.h \
    globaldefine.h \
    server/serverMIIO.h \
    server/serverSync.h \
    server/serverHodor.h \
    hodor/hodorControl.h \
    board/testItem/baseTestItem.h \
    board/testItem/testCPU.h \
    board/testItem/testGravity.h \
    board/testItem/testMIIO.h \
    board/testItem/testRTC.h \
    board/testItem/testUSB.h \
    board/testItem/testVOL.h \
    board/testItem/testWiFi.h \
    widget/widgetMain.h \
    widget/hodor/widgetHodor.h \
    widget/board/widgetControl.h \
    widget/board/widgetSetting.h \
    widget/board/base/statusMore.h \
    widget/board/base/statusTest.h \
    widget/board/base/widgetDebugInfo.h \
    widget/board/item/widgetTest.h \
    file/fileConfig.h \
    file/fileHodor.h \
    control/deviceControl.h \
    control/serialControl.h \
    board/deviceItem.h \
    board/serialItem.h \
    widget/board/item/widgetItem.h \
    board/testControl.h \
    control/sysData.h \
    board/testItem/testSYNC.h

FORMS    += widget/widgetMain.ui \
    widget/hodor/widgetHodor.ui \
    widget/board/widgetControl.ui \
    widget/board/widgetSetting.ui \
    widget/board/base/statusMore.ui \
    widget/board/base/statusTest.ui \
    widget/board/base/widgetDebugInfo.ui \
    widget/board/item/widgetTest.ui \
    widget/board/item/widgetTestItem.ui


INCLUDEPATH +=

RESOURCES += \
    resource/resource.qrc
