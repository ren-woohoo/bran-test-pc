#ifndef TESTRTC_H
#define TESTRTC_H

#include <QDebug>
#include "baseTestItem.h"

class TestRTC : public BaseTestItem
{
    Q_OBJECT
public:
    explicit TestRTC(DeviceItem *,SerialItem*);
    void start_test();

};

#endif // TESTRTC_H
