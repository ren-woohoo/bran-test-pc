#ifndef HODORCONTROL_H
#define HODORCONTROL_H

#include <QThread>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QTimer>
#include "device/deviceItem.h"
#include "server/serverHodor.h"
#include "server/serverUser.h"
#include "globaldefine.h"

#define TEST_TEMP_HUMI   "TEST_TEMP_HUMI"
#define TEST_TVOC_CO2E   "TEST_TVOC_CO2E"
#define TEST_PM25        "TEST_PM25"
#define TEST_BUTTON      "TEST_BUTTON"
#define TEST_WIFI        "TEST_WIFI"
#define TEST_GRAVITY     "TEST_GRAVITY"
#define TEST_TOUCH       "TEST_TOUCH"
#define TEST_BATTERY     "TEST_BATTERY"
#define TEST_LIGHT       "TEST_LIGHT"

class HodorControl : public QThread
{
    Q_OBJECT
public:
    static HodorControl *getInstance();

signals:
    void signal_update_device(InfoDevice, InfoHodor);
    void signal_remove_device();
    void signal_sync_testPassed(InfoDevice, InfoHodor);
    void signal_sync_testFailed(InfoDevice, InfoHodor);
    void signal_syncTest_success();
    void signal_syncTest_failed(QString);

private slots:
    void slot_add_devices(QList<QString>);
    void slot_delete_devices(QList<QString>);
    void slot_refresh_device();
    void slot_sync_hodor();

private:
    void run();
    void data_init();
    void connect_init();
    bool refresh_device(QString);


private:
    DeviceItem *deviceItem;
    ServerHodor *serverHodor;
    ServerUser *serverUser;
    InfoDevice infoDevice;
    InfoHodor infoHodor;

    QList<QString> listDevicesCache;
    bool isFree;

    QTimer *timer;

private:
    static HodorControl *instance;
    explicit HodorControl();
};

#endif // HODORCONTROL_H
