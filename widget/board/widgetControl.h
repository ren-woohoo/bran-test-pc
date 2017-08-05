#ifndef WIDGETCONTROL_H
#define WIDGETCONTROL_H

#include <QWidget>
#include <QTimer>
#include "board/testControl.h"
#include <QMessageBox>

namespace Ui {
class WidgetControl;
}

class WidgetControl : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetControl(QWidget *parent = 0);
    ~WidgetControl();

signals:
    void signal_start_test();
    void signal_close_ports();

private slots:
    void slot_add_devices(QList<QString>);
    void slot_delete_devices(QList<QString>);
    void slot_add_ports(QList<QString>);
    void slot_delete_ports(QList<QString>);
    void slot_refresh_cache();
    void slot_test_end();
    void on_button_start_clicked();

    void slot_openPort_failed(QString);

private:
    void data_init();
    void connect_init();

private:
    Ui::WidgetControl *ui;

    TestControl *testItem1;
    TestControl *testItem2;
    TestControl *testItem3;
    TestControl *testItem4;
    TestControl *testItem5;
    TestControl *testItem6;

    QList<TestControl*> listTestItem;
    QList<QString> listDevicesCache;
    QList<QString> listPortsCache;

    QTimer *timerRefresh;

};

#endif // WIDGETCONTROL_H
