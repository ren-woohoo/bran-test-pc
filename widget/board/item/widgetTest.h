#ifndef WIDGETTEST_H
#define WIDGETTEST_H

#include <QWidget>
#include "widget/base/statusTest.h"
#include "widget/base/statusMore.h"
#include "widget/base/widgetDebugInfo.h"

namespace Ui {
class WidgetTest;
}

class WidgetTest : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTest(QWidget *parent = 0);
    ~WidgetTest();
    void start_test();

signals:

private slots:
    void slot_testCPU_result(int, QString);
    void slot_testRTC_result(int, QString);
    void slot_testGravity_result(int, QString);
    void slot_testWiFi_result(int, QString);
    void slot_testUSB_result(int, QString);
    void slot_testVOL_result(int, QString);
    void slot_testMIIO_result(int, QString);
    void slot_syncTest_result(int, QString);

    void slot_testCPU_debugInfo();
    void slot_testRTC_debugInfo();
    void slot_testGravity_debugInfo();
    void slot_testWiFi_debugInfo();
    void slot_testUSB_debugInfo();
    void slot_testVOL_debugInfo();
    void slot_testMIIO_debugInfo();
    void slot_syncTest_debugInfo();

private:
    void data_init();
    void connect_init();
    void widget_init();

private:
    Ui::WidgetTest *ui;

    WidgetDebugInfo *widgetDebugInfo;

    StatusMore *statusMore1;
    StatusMore *statusMore2;
    StatusMore *statusMore3;
    StatusMore *statusMore4;
    StatusMore *statusMore5;
    StatusMore *statusMore6;
    StatusMore *statusMore7;
    StatusMore *statusMore8;

    StatusTest *statusTest1;
    StatusTest *statusTest2;
    StatusTest *statusTest3;
    StatusTest *statusTest4;
    StatusTest *statusTest5;
    StatusTest *statusTest6;
    StatusTest *statusTest7;
    StatusTest *statusTest8;

    QString debugInfoCPU;
    QString debugInfoRTC;
    QString debugInfoGravity;
    QString debugInfoWiFi;
    QString debugInfoUSB;
    QString debugInfoVOL;
    QString debugInfoMIIO;
    QString debugInfoSync;

    bool isFailed;
};

#endif // WIDGETTEST_H
