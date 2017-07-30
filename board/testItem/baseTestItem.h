#ifndef BASETESTITEM_H
#define BASETESTITEM_H

#include <QDebug>
#include "device/deviceItem.h"
#include "serial/serialItem.h"
#include "globaldefine.h"

class BaseTestItem : public QObject
{
    Q_OBJECT
public:
    explicit BaseTestItem(DeviceItem*,SerialItem*);
    QString get_debugInfo();
    void start_test();

signals:
    void signal_test_result(int,QString);

public slots:

protected:
    DeviceItem *deviceItem;
    SerialItem *serialItem;

    QString debugInfo;
};

#endif // BASETESTITEM_H
