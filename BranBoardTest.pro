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

DEFINES += TEST_MODE
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
        widgetMain.cpp \
    server/serverUser.cpp \
    server/networkManager.cpp \
    system/sysUser.cpp \
    widget/widgetTest.cpp \
    widget/widgetSetting.cpp \
    widget/base/statusMore.cpp \
    widget/base/statusTest.cpp \
    widget/base/widgetDebugInfo.cpp \
    file/fileConfig.cpp \
    widget/widgetControl.cpp \
    serial/serialControl.cpp \
    device/deviceControl.cpp \
    widget/base/widgetTestItem.cpp \
    serial/serialItem.cpp \
    device/deviceItem.cpp \
    test/testCPU.cpp \
    test/testUSB.cpp \
    test/testGravity.cpp \
    test/testRTC.cpp \
    test/testWiFi.cpp \
    test/baseTestItem.cpp \
    test/testVOL.cpp \
    test/testMIIO.cpp \
    server/serverMIIO.cpp \
    server/serverSync.cpp \
    system/sysData.cpp \
    test/testControl.cpp \
    file/fileHodor.cpp \
    server/serverHodor.cpp \
    widget/widgetHodor.cpp \
    hodor/hodorControl.cpp

HEADERS  += widgetMain.h \
    server/serverUser.h \
    server/networkManager.h \
    system/sysUser.h \
    globaldefine.h \
    widget/widgetTest.h \
    widget/widgetSetting.h \
    widget/base/statusMore.h \
    widget/base/statusTest.h \
    widget/base/widgetDebugInfo.h \
    file/fileConfig.h \
    widget/widgetControl.h \
    serial/serialControl.h \
    device/deviceControl.h \
    widget/base/widgetTestItem.h \
    serial/serialItem.h \
    device/deviceItem.h \
    test/testUSB.h \
    test/testGravity.h \
    test/testRTC.h \
    test/testWiFi.h \
    test/baseTestItem.h \
    test/testVOL.h \
    test/testMIIO.h \
    server/serverMIIO.h \
    server/serverSync.h \
    test/testCPU.h \
    system/sysData.h \
    test/testControl.h \
    file/fileHodor.h \
    server/serverHodor.h \
    widget/widgetHodor.h \
    hodor/hodorControl.h

FORMS    += widgetMain.ui \
    widget/widgetTest.ui \
    widget/widgetSetting.ui \
    widget/base/statusMore.ui \
    widget/base/statusTest.ui \
    widget/base/widgetDebugInfo.ui \
    widget/widgetControl.ui \
    widget/base/widgetTestItem.ui \
    widget/widgetHodor.ui


INCLUDEPATH +=

RESOURCES += \
    resource/resource.qrc
