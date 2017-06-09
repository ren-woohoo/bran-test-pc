#ifndef SYSDATA_H
#define SYSDATA_H

#include <QThread>
#include "file/fileConfig.h"

class SysData : public QThread
{
    Q_OBJECT
public:
    static SysData* getInstance();
    RefDatas get_refDatas();

signals:
    void signal_update_refDatas(RefDatas);

private slots:
    void slot_change_refDatas(RefDatas);

private:
    void data_init();
    void connect_init();

private:
    FileConfig *fileConfig;

    RefDatas refDatas;

private:
    SysData();
    static SysData* instance;
};

#endif // SYSDATA_H
