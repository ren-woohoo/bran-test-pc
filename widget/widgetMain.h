#ifndef WIDGETMAIN_H
#define WIDGETMAIN_H

#include <QWidget>
#include <QTimer>

#include "system/sysUser.h"
#include "widget/widgetTest.h"
#include "widget/widgetControl.h"
#include "widget/widgetSetting.h"
#include "widget/widgetHodor.h"
#include "device/deviceControl.h"

#include "device/deviceControl.h"
#include "serial/serialControl.h"
#include "hodor/hodorControl.h"

namespace Ui {
class WidgetMain;
}

class WidgetMain : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetMain(QWidget *parent = 0);
    ~WidgetMain();

signals:
    void signal_user_login(QString,QString);                            // 用户登录信号
    void signal_user_modify(QString);                                   // 用户修改密码
    void signal_userLogin_success();                                    // 用户登录成功

private slots:
    void slot_login_result(int);
    void slot_modify_result(int);
    void on_button_passwordModify_clicked();
    void on_button_backModify_clicked();
    void on_button_login_clicked();
    void on_button_pageModify_clicked();
    void on_edit_newPassword2_editingFinished();
    void slot_quit_program();

private:
    void data_init();
    void connect_init();
    void widget_init();

private:
    Ui::WidgetMain *ui;
    SysUser *sysUser;

    DeviceControl *deviceControl;
    SerialControl *serialControl;
    HodorControl *hodorControl;

    WidgetControl *widgetControl;
    WidgetSetting *widgetSetting;
    WidgetHodor *widgetHodor;
    QTimer *timerQuit;
};

#endif // WIDGETMAIN_H
