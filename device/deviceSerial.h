#ifndef DEVICESERIAL_H
#define DEVICESERIAL_H

#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QStringList>
#include <QString>
#include <QMutex>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "globaldefine.h"

class DeviceSerial : public QThread
{
    Q_OBJECT
public:
    static DeviceSerial *getInstance();
    QList<QString> get_listPort();

signals:
    void signal_closePort_result(int);
    void signal_openPort_result(int);
    void signal_refresh_listPort(QList<QString>);

    void signal_burnPrepare_feedback(QString);
    void signal_burnStart_feedback(QString);
    void signal_burnEnd_feedback(QString);
    void signal_usbA0B0_feedback(QString);
    void signal_usbA0B1_feedback(QString);
    void signal_usbA1B0_feedback(QString);
    void signal_usbA1B1_feedback(QString);
    void signal_getVoltage_feedback(QString);

private slots:
    void slot_open_port(QString);
    void slot_close_port();
    void slot_refresh_port();
    void slot_change_portName(QString);
    void slot_recive_data();
    void slot_read_data();
    void slot_write_data(CommandType,int len = 0, QString data ="");


private:
    void data_init();
    void connect_init();

private:
    QSerialPort *serialPort;
    QList<QString> listPort;
    QTimer *timerRefresh;
    QTimer *timerDelay;

    QByteArray dataCache;

    QString portName;
    QMutex mutex;

private:
    DeviceSerial();
    static DeviceSerial *instance;
};


#endif // DEVICESERIAL_H
