#ifndef TESTVOL_H
#define TESTVOL_H

#include "control/sysData.h"
#include "baseTestItem.h"

class TestVOL : public BaseTestItem
{
    Q_OBJECT
public:
    explicit TestVOL(DeviceItem*, SerialItem*);
    void start_test();

signals:
    void signal_update_infoVOL(InfoVol);

private slots:
    void slot_getVoltage_feedback(QString,QString);
    void slot_change_refDatas(RefDatas);

    void slot_serial_timeout();

private:
    bool analyse_data(InfoVol);
    bool is_valid(float,float);
    void data_init();
    void connect_init();

private:
    InfoVol infoVol;
    RefDatas refDatas;
    QTimer *timer;

    SysData *sysData;

    bool isFailed;
};

#endif // TESTVOL_H
