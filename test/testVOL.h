#ifndef TESTVOL_H
#define TESTVOL_H

#include "system/sysData.h"
#include "baseTestItem.h"

class TestVOL : public BaseTestItem
{
    Q_OBJECT
public:
    explicit TestVOL(DeviceItem*, SerialItem*);
    void start_test();

signals:

private slots:
    void slot_getVoltage_feedback(QString);
    void slot_getVoltage_timeout();

    void slot_change_refDatas(RefDatas);

private:
    bool analyse_data(InfoVol);
    bool is_valid(float,float);
    void data_init();
    void connect_init();

private:
    InfoVol branData;
    RefDatas refDatas;
    QTimer *timer;
    int numFailed;

    SysData *sysData;
};

#endif // TESTVOL_H
