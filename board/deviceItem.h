#ifndef DEVICEITEM_H
#define DEVICEITEM_H

#include <QProcess>
#include <QDebug>
#include <stdio.h>

enum adbCMD
{
    GET_SN = 0,

};

class DeviceItem : public QObject
{
    Q_OBJECT
public:
    explicit DeviceItem(QObject *parent = 0);
    void set_device(QString);

    QString adb_shell(QString);
    void adb_push(QString);
    QString excute_cmd(QString);

signals:
    void signal_feedback(QString);

private slots:

private:
    QString device;

};

#endif // DEVICEITEM_H
