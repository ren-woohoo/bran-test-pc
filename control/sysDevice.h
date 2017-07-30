#ifndef DEVICECONTROL_H
#define DEVICECONTROL_H

#include <QProcess>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QStringList>

class DeviceControl : public QThread
{
    Q_OBJECT
public:
    static DeviceControl *getInstance();

signals:
    void signal_add_devices(QList<QString>);
    void signal_delete_devices(QList<QString>);
    void signal_update_devices(QList<QString>);

private slots:
    void slot_update_devices();

private:
    void data_init();
    void connect_init();
    void run();

private:
    QTimer *timerRefresh;

    QList<QString> listDevices;

private:
    static DeviceControl *instance;
    explicit DeviceControl();
};

#endif // DEVICECONTROL_H
