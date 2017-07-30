#ifndef TESTCPU_H
#define TESTCPU_H

#include <QDebug>
#include "baseTestItem.h"

class TestCPU : public BaseTestItem
{
    Q_OBJECT
public:
    explicit TestCPU(DeviceItem*,SerialItem*);
    void start_test();

    void set_deviceADB(QString);
    void set_deviceSN(QString);

private:
    QString deviceADB;
    QString deviceSN;

};

#endif // TESTCPU_H
