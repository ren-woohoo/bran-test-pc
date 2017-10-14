#ifndef TestControl_H
#define TestControl_H

#include <QThread>

#include "widget/board/item/widgetItem.h"
#include "widget/board/item/widgetTest.h"
#include "deviceItem.h"
#include "serialItem.h"

#include "server/serverMIIO.h"
#include "server/serverSync.h"
#include "server/serverUser.h"

#include "testItem/testCPU.h"
#include "testItem/testRTC.h"
#include "testItem/testGravity.h"
#include "testItem/testWiFi.h"
#include "testItem/testUSB.h"
#include "testItem/testVOL.h"
#include "testItem/testMIIO.h"
#include "testItem/testSYNC.h"
#include <QFile>

enum TestStage{

    IS_FREE = 0,
    IS_PORT_READY = 1,
    IS_FIXTURE_READY = 2,
    IS_DEVICE_READY = 3,
    IS_MATCHED = 4,
    IS_TESTING = 5,
};

class TestControl : public QThread
{
    Q_OBJECT
public:
    explicit TestControl(QWidget *parent = 0);
    TestStage testStage;
    QString get_port();
    QString get_device();
    QString get_deviceSN();

public:
    void load_port(QString);
    void remove_port();
    void load_fixture(QString);
    void sampling_deviceSN();
    void load_deviceSN(QString);
    void load_deviceADB(QString);
    void remove_device();

signals:
    void signal_test_end();
    void signal_test_init();

    void signal_open_port(QString);
    void signal_close_port();
    void signal_openPort_failed(QString);

    void signal_write_data(QString);
    void signal_update_infoFixture(InfoFixture);
    void signal_update_progress(int);
    void signal_test_result(int);

    void signal_testCPU_result(int,QString);
    void signal_testRTC_result(int,QString);
    void signal_testGravity_result(int,QString);
    void signal_testWiFi_result(int,QString);
    void signal_testUSB_result(int,QString);
    void signal_testVOL_result(int,QString);
    void signal_testMIIO_result(int,QString);
    void signal_testSYNC_result(int,QString);

private slots:
    void slot_getDevice_feedback(QString,QString);
    void slot_getFixture_feedback(QString,QString);
    void slot_update_infoMIIO(InfoMIIO);
    void slot_update_infoVOL(InfoVol);
    void slot_start_test();
    void slot_retry_samplingFixture();
    void slot_testCPU_result(int,QString);
    void slot_testRTC_result(int,QString);
    void slot_testGravity_result(int,QString);
    void slot_testWiFi_result(int,QString);
    void slot_testUSB_result(int,QString);
    void slot_testVOL_result(int,QString);
    void slot_testMIIO_result(int,QString);
    void slot_testSYNC_result(int,QString);

    void slot_openPort_result(int);
    void slot_close_port();

private:
    void data_init();
    void connect_init();

private:
    WidgetTestItem *widgetItem;
    WidgetTest  *widgetTest;

    SerialItem *serialItem;
    DeviceItem *deviceItem;
    ServerSync *serverSync;
    ServerMIIO *serverMIIO;
    ServerUser *serverUser;

    TestCPU *testCPU;
    TestRTC *testRTC;
    TestGravity *testGravity;
    TestWiFi *testWiFi;
    TestUSB *testUSB;
    TestVOL *testVOL;
    TestMIIO *testMIIO;
    TestSYNC *testSYNC;

    int progress;
    bool isTesting;

    InfoTest infoTest;
    QTimer *timerRetry;
    QString cmd;
};

#endif // TestControl_H
