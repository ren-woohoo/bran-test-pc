#ifndef TESTUSB_H
#define TESTUSB_H

#include <QTime>
#include <QDebug>
#include <QTimer>
#include <QEventLoop>
#include "baseTestItem.h"

#define DEVICE_POWER_AC         "/sys/class/power_supply/ac/present"          //检测是否底座充电 1 底座充电 0 无底座充电
#define DEVICE_POWER_USB        "/sys/class/power_supply/usb/present"         //检测是否USB充电  1 USB充电 0 无USB

class TestUSB : public BaseTestItem
{
    Q_OBJECT
public:
    explicit TestUSB(DeviceItem*,SerialItem*);
    void start_test();

private slots:
    void slot_usbA0B1_feedback(QString,QString);
    void slot_usbA1B0_feedback(QString,QString);
    void slot_usbA1B1_feedback(QString,QString);
    void slot_serial_timeout();

private:
    bool update_ac(bool&);
    bool update_usb(bool&);

private:
    QTime timeKeep;
    QEventLoop *loop;
    QTimer *timer;

    bool ac;
    bool usb;
};

#endif // TESTUSB_H
