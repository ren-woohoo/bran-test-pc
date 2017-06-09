#ifndef TESTGRAVITY_H
#define TESTGRAVITY_H

#include "baseTestItem.h"
#include "globaldefine.h"

class TestGravity : public BaseTestItem
{
    Q_OBJECT
public:
    explicit TestGravity(DeviceItem*,SerialItem*);
    void start_test();

signals:

private:

};

#endif // TESTGRAVITY_H
