#ifndef WIDGETHODOR_H
#define WIDGETHODOR_H

#include <QWidget>
#include "hodor/hodorControl.h"
#include "globaldefine.h"
#include "base/statusTest.h"

namespace Ui {
class WidgetHodor;
}

class WidgetHodor : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetHodor(QWidget *parent = 0);
    ~WidgetHodor();

signals:
    void signal_sync_hodor();

private slots:
    void on_button_sync_clicked();
    void slot_update_device(InfoDevice, InfoHodor);
    void slot_remove_device();
    void slot_syncTest_success();
    void slot_syncTest_failed(QString);

private:
    void data_init();
    void connect_init();


private:
    StatusTest *statusTest1;
    StatusTest *statusTest2;
    StatusTest *statusTest3;
    StatusTest *statusTest4;
    StatusTest *statusTest5;
    StatusTest *statusTest6;
    StatusTest *statusTest7;
    StatusTest *statusTest8;
    StatusTest *statusTest9;

    QList<QString> listPorts;

    HodorControl *hodorControl;

private:
    Ui::WidgetHodor *ui;
};

#endif // WIDGETHODOR_H
