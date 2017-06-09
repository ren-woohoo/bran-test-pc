#ifndef SERIALITEM_H
#define SERIALITEM_H

#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QStringList>
#include <QString>
#include <QMutex>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "globaldefine.h"

class SerialItem : public QSerialPort
{
    Q_OBJECT
public:
    explicit SerialItem(QObject *parent = 0);
    void set_port(QString);
    void open_port();
    void close_port();
    void write_data(CommandType,int len = 0, QString data ="");

signals:
    void signal_closePort_result(int);
    void signal_openPort_result(int);

    void signal_burnPrepare_feedback(QString);
    void signal_burnStart_feedback(QString);
    void signal_burnEnd_feedback(QString);
    void signal_usbA0B0_feedback(QString);
    void signal_usbA0B1_feedback(QString);
    void signal_usbA1B0_feedback(QString);
    void signal_usbA1B1_feedback(QString);

    void signal_getVoltage_feedback(QString);
    void signal_getDevice_feedback(QString);
    void signal_getFixture_feedback(QString);

private slots:
    void slot_recive_data();
    void slot_read_data();

private:
    void data_init();
    void connect_init();

private:
    QString port;
    QByteArray dataCache;
    QTimer *timerDelay;
};

#endif // SERIALITEM_H
