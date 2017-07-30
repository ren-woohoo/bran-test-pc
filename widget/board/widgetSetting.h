#ifndef WIDGETSETTING_H
#define WIDGETSETTING_H

#include <QWidget>
#include <QMessageBox>
#include <QStyledItemDelegate>
#include "file/fileConfig.h"
#include "control/sysData.h"

namespace Ui {
class WidgetSetting;
}

class WidgetSetting : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetSetting(QWidget *parent = 0);
    ~WidgetSetting();

signals:
    void signal_change_refDatas(RefDatas);

private slots:
    void on_button_change_clicked();

private:
    void data_init();
    void connect_init();
    void widget_init();
    float get_validValue(QString);

private:
    Ui::WidgetSetting *ui;
    RefDatas refDatas;
    SysData *sysData;
    bool isOpen;
};

#endif // WIDGETSETTING_H
