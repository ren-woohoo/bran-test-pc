#ifndef TESTSYNC_H
#define TESTSYNC_H

#include "baseTestItem.h"

class TestSYNC : public BaseTestItem
{
    Q_OBJECT
public:
    explicit TestSYNC(DeviceItem*,SerialItem*);
    void start_test(InfoTest);

signals:
    void signal_sync_testPassed(InfoTest);
    void signal_sync_testFailed(InfoTest);

private slots:
    void slot_syncTest_success(QString,QString);
    void slot_syncTest_failed(QString,QString);

};

#endif // TESTSYNC_H
