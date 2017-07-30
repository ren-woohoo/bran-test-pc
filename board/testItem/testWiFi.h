#ifndef TESTWIFI_H
#define TESTWIFI_H

#include <QEventLoop>
#include <QTimer>
#include "baseTestItem.h"

class TestWiFi : public BaseTestItem
{
    Q_OBJECT
public:
    explicit TestWiFi(DeviceItem*,SerialItem*);
    void start_test();

signals:

public slots:
};

#endif // TESTWIFI_H
