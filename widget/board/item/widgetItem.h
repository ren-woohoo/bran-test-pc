#ifndef WIDGETTESTITEM_H
#define WIDGETTESTITEM_H

#include <QWidget>
#include <QEventLoop>
#include <QTimer>
#include "test/testControl.h"
#include "widget/widgetTest.h"

namespace Ui {
class WidgetTestItem;
}

enum TestStage{

    FREE = 0,
    LOADED = 1,
    CONNECTED = 2,
    READY = 3,
    MATCHED = 4,
    STAGE_TESTING = 5,
    STAGE_TESTED = 6
};

class WidgetTestItem : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTestItem(QWidget *parent = 0);
    ~WidgetTestItem();

    TestStage get_status();

    void start_test();

    void load_fixture(QString);
    void remove_fixture();
    QString get_fixture();

    void load_device(QString);
    void remove_device();
    QString get_device();

    QString get_deviceSN();

    void sample_device();

signals:
    void signal_set_device(QString);
    void signal_open_port();
    void signal_start_test();
    void signal_stop_test();
    void signal_connect_fixture(QString);
    void signal_sample_deviceSN();
    void signal_sample_fixture();
    void signal_test_end();
    void signal_match_device(QString);

private slots:
    void slot_update_fixtureSN(QString);
    void slot_update_deviceSN(QString);
    void slot_update_progress(int);
    void slot_refresh_progress();

    void slot_test_result(int);
    void on_button_more_clicked();

private:
    void data_init();
    void connect_init();
    void page_init();

private:
    QString port;
    QString device;
    int progress;

    QString fixtureSN;
    QString deviceSN;

    TestStage status;
    QTimer *timerProgress;

private:
    Ui::WidgetTestItem *ui;
    TestControl *testControl;
    WidgetTest *widgetTest;
};

#endif // WIDGETTESTITEM_H
