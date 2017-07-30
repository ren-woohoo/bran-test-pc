#ifndef WIDGETTESTITEM_H
#define WIDGETTESTITEM_H

#include <QWidget>
#include <QEventLoop>
#include <QTimer>
#include <QDebug>
#include "globaldefine.h"

namespace Ui {
class WidgetTestItem;
}

class WidgetTestItem : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTestItem(QWidget *parent = 0);
    ~WidgetTestItem();

signals:
    void signal_show_widgetTest();

private slots:
    void slot_test_init();
    void slot_update_infoFixture(InfoFixture);
    void slot_update_progress(int);
    void slot_refresh_progress();

    void slot_test_result(int);
    void on_button_more_clicked();

private:
    void data_init();
    void connect_init();
    void page_init();

private:
    int progress;
    QTimer *timerProgress;

private:
    Ui::WidgetTestItem *ui;
};

#endif // WIDGETTESTITEM_H
