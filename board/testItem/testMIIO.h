#ifndef TESTMIIO_H
#define TESTMIIO_H

#include "baseTestItem.h"
#include "server/serverMIIO.h"

class TestMIIO : public BaseTestItem
{
    Q_OBJECT
public:
    explicit TestMIIO(DeviceItem*,SerialItem*);
    void start_test();
    void set_deviceSN(QString);

signals:
    void signal_get_infoMIIO(QString);
    void signal_update_infoMIIO(InfoMIIO);

private slots:
    void slot_getInfoMIIO_success(QString,InfoMIIO);
    void slot_getInfoMIIO_failed(QString,QString);

private:
    QString deviceSN;

    ServerMIIO *serverMIIO;

};

#endif // TESTMIIO_H
