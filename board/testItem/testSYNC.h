#ifndef TESTSYNC_H
#define TESTSYNC_H

#include <QFile>
#include <QMutex>
#include "baseTestItem.h"

class TestSYNC : public BaseTestItem
{
    Q_OBJECT
public:
    explicit TestSYNC(DeviceItem*,SerialItem*);
    void start_test(InfoTest);
    void set_device(QString);

signals:
    void signal_sync_testPassed(InfoTest);
    void signal_sync_testFailed(InfoTest);

private slots:
    void slot_syncTest_success(QString,QString);
    void slot_syncTest_failed(QString,QString);

private:
    QString deviceADB;
    InfoTest infoTest;
    QMutex mutex;

};

#endif // TESTSYNC_H
