#ifndef SERIALCONTROL_H
#define SERIALCONTROL_H

#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class SerialControl : public QThread
{
    Q_OBJECT
public:
    static SerialControl *getInstance();

signals:
    void signal_add_ports(QList<QString>);
    void signal_delete_ports(QList<QString>);

private slots:
    void slot_refresh_ports();

private:
    void data_init();
    void connect_init();
    void run();

private:
    QTimer *timerRefresh;
    QList<QString> listPort;

private:
    static SerialControl *instance;
    explicit SerialControl(QObject *parent = 0);
};

#endif // SERIALCONTROL_H
