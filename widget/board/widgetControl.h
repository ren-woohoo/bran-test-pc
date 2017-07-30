#ifndef WIDGETCONTROL_H
#define WIDGETCONTROL_H

#include <QWidget>
#include <QTimer>
#include "base/widgetTestItem.h"
#include "serial/serialControl.h"
#include "device/deviceControl.h"

namespace Ui {
class WidgetControl;
}

class WidgetControl : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetControl(QWidget *parent = 0);
    ~WidgetControl();

private slots:
    void slot_add_devices(QList<QString>);
    void slot_delete_devices(QList<QString>);
    void slot_add_ports(QList<QString>);
    void slot_delete_ports(QList<QString>);

    void slot_match_device(QString);

    void on_button_start_clicked();

    void slot_verify_device();
    void slot_test_end();

private:
    void data_init();
    void connect_init();

private:
    Ui::WidgetControl *ui;

    WidgetTestItem *widgetTestItem1;
    WidgetTestItem *widgetTestItem2;
    WidgetTestItem *widgetTestItem3;
    WidgetTestItem *widgetTestItem4;
    WidgetTestItem *widgetTestItem5;
    WidgetTestItem *widgetTestItem6;

    QList<WidgetTestItem*> listTestItem;
    QList<QString> listDevicesCache;

    QTimer *timerVerify;

};

#endif // WIDGETCONTROL_H
