#ifndef TestControl_H
#define TestControl_H

#include <QThread>

#include "serial/serialItem.h"
#include "device/deviceItem.h"
#include "server/serverMIIO.h"
#include "server/serverSync.h"
#include "server/serverUser.h"

#include "testCPU.h"
#include "testRTC.h"
#include "testGravity.h"
#include "testWiFi.h"
#include "testUSB.h"
#include "testVOL.h"
#include "testMIIO.h"

class TestControl : public QThread
{
    Q_OBJECT
public:
    explicit TestControl();

signals:
    void signal_connectFixture_result(int);
    void signal_update_fixtureSN(QString);
    void signal_update_deviceSN(QString);
    void signal_update_progress(int);
    void signal_sync_testPassed(InfoTest);
    void signal_sync_testFailed(InfoTest);

    void signal_test_result(int);
    void signal_testCPU_result(int,QString);
    void signal_testRTC_result(int,QString);
    void signal_testGravity_result(int,QString);
    void signal_testWiFi_result(int,QString);
    void signal_testUSB_result(int,QString);
    void signal_testVOL_result(int,QString);
    void signal_testMIIO_result(int,QString);
    void signal_syncTest_result(int,QString);

private slots:
    void slot_start_test();
    void slot_stop_test();

    void slot_set_deviceADB(QString);
    void slot_connect_fixture(QString);
    void slot_disconnect_fixture();

    void slot_sample_deviceSN();
    void slot_openPort_result(int);
    void slot_closePort_result(int);

    void slot_getDevice_feedback(QString);
    void slot_getFixture_feedback(QString);

    void slot_testCPU_result(int,QString);
    void slot_testRTC_result(int,QString);
    void slot_testGravity_result(int,QString);
    void slot_testWiFi_result(int,QString);
    void slot_testUSB_result(int,QString);
    void slot_testVOL_result(int,QString);
    void slot_testMIIO_result(int,QString);
    void slot_syncTest_success();
    void slot_syncTest_failed(QString);

    void slot_update_infoMIIO(InfoMIIO);

private:
    void data_init();
    void connect_init();

private:
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

    QString fixtureSN;
    QString deviceSN;

    QString port;
    QString deviceADB;

    QString debugInfoSync;

    bool isFailed;
    int progress;

    InfoTest infoTest;
};

#endif // TestControl_H
