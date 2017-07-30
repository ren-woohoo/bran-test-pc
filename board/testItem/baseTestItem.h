#ifndef BASETESTITEM_H
#define BASETESTITEM_H

#include <QDebug>
#include <QStringList>
#include <QString>
#include "board/deviceItem.h"
#include "board/serialItem.h"
#include "globaldefine.h"

class BaseTestItem : public QObject
{
    Q_OBJECT
public:
    explicit BaseTestItem(DeviceItem*,SerialItem*);
    QString get_debugInfo();
    void start_test();

signals:
    void signal_write_data(QString);
    void signal_test_result(int,QString);

public slots:

protected:
    DeviceItem *deviceItem;
    SerialItem *serialItem;

    QString debugInfo;

    CommandType cmdType;
    QString cmd;
};

#endif // BASETESTITEM_H
