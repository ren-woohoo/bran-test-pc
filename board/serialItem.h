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
    QString package_cmd(CommandType, QString data = "");
    bool open_port(QString);
    void close_port();

signals:
    void signal_usbA0B0_feedback(QString,QString);
    void signal_usbA0B1_feedback(QString,QString);
    void signal_usbA1B0_feedback(QString,QString);
    void signal_usbA1B1_feedback(QString,QString);

    void signal_getVoltage_feedback(QString,QString);
    void signal_getDevice_feedback(QString,QString);
    void signal_getFixture_feedback(QString,QString);

    void signal_openPort_result(int);

private slots:

    void slot_recive_data();
    void slot_read_data();
    void slot_write_data(QString);

private:
    void data_init();
    void connect_init();
    void analyse_reply(QString);
    QString package_cmd(QString, QString);

private:
    QString port;
    QByteArray dataCache;
    QTimer *timerDelay;
};

#endif // SERIALITEM_H
