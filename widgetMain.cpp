#include "widgetMain.h"
#include "ui_widgetMain.h"

/*******************************************************************************
* Function Name  :  WidgetMain
* Description    :  主窗口
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WidgetMain::WidgetMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetMain)
{
    ui->setupUi(this);
    data_init();
    widget_init();
    connect_init();
    deviceControl->start();
    serialControl->start();
}

/*******************************************************************************
* Function Name  :  ~WidgetMain
* Description    :  析构函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WidgetMain::~WidgetMain()
{
    delete ui;
}

/*******************************************************************************
* Function Name  :  data_init
* Description    :  数据初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetMain::data_init()
{
    sysUser       = SysUser::getInstance();
    deviceControl = DeviceControl::getInstance();
    serialControl = SerialControl::getInstance();
    hodorControl  = HodorControl::getInstance();
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetMain::connect_init()
{
    connect(this, SIGNAL(signal_user_login(QString,QString)), sysUser, SIGNAL(signal_user_login(QString,QString)));
    connect(sysUser, SIGNAL(signal_login_result(int)), this, SLOT(slot_login_result(int)));
    connect(this, SIGNAL(signal_user_modify(QString)), sysUser, SIGNAL(signal_user_modify(QString)));
    connect(sysUser, SIGNAL(signal_modify_result(int)), this, SLOT(slot_modify_result(int)));

    connect(deviceControl, SIGNAL(signal_add_devices(QList<QString>)), widgetControl, SLOT(slot_add_devices(QList<QString>)));
    connect(deviceControl, SIGNAL(signal_delete_devices(QList<QString>)), widgetControl, SLOT(slot_delete_devices(QList<QString>)));

    connect(serialControl, SIGNAL(signal_add_ports(QList<QString>)), widgetControl, SLOT(slot_add_ports(QList<QString>)));
    connect(serialControl, SIGNAL(signal_delete_ports(QList<QString>)), widgetControl, SLOT(slot_delete_ports(QList<QString>)));

    connect(deviceControl, SIGNAL(signal_add_devices(QList<QString>)), hodorControl, SLOT(slot_add_devices(QList<QString>)));
    connect(deviceControl, SIGNAL(signal_delete_devices(QList<QString>)), hodorControl, SLOT(slot_delete_devices(QList<QString>)));
}

/*******************************************************************************
* Function Name  :  widget_init
* Description    :  窗口初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetMain::widget_init()
{

#ifdef TEST_MODE
    ui->label_title->setText(QStringLiteral("Bran工装上位机(测试版v1.1)"));
    ui->label_smallTitle->setText(QStringLiteral("测试版v1.1"));
#else
    ui->label_title->setText(QStringLiteral("Bran工装上位机(正式版v1.1)"));
    ui->label_smallTitle->setText(QStringLiteral("正式版v1.1"));
#endif
    widgetControl = new WidgetControl(ui->widget_test);
    widgetSetting = new WidgetSetting(ui->widget_setting);
    widgetHodor = new WidgetHodor(ui->tabWidget_hodor);

    ui->stackedWidget_statusLogin->setCurrentWidget(ui->page_loginNormal);
    ui->stackedWidget_statusModify->setCurrentWidget(ui->page_modifyNormal);

//    ui->stackedWidget_main->setCurrentWidget(ui->page_content);
}

/*******************************************************************************
* Function Name  :  on_button_login_clicked
* Description    :  登录按钮点击
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetMain::on_button_login_clicked()
{
    // 将状态置为正在登录,登录按钮不让重复点击
    ui->stackedWidget_statusLogin->setCurrentWidget(ui->page_logining);
    ui->button_login->setEnabled(false);

    QString user = ui->edit_user->text();
    QString password = ui->edit_password->text();

    // 发送登录信号
    qDebug()<<"##########################";
    emit signal_user_login(user,password);
}

/*******************************************************************************
* Function Name  :  slot_login_result()
* Description    :  登录结果反馈
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetMain::slot_login_result(int result)
{
    ui->button_login->setEnabled(true);
    if(0 == result)
    {
        // 登录成功
        emit signal_userLogin_success();
        // 跳转页面,到内容页面
        ui->stackedWidget_main->setCurrentWidget(ui->page_content);
    }
    else if(-2 == result)
    {
        // 密码错误，在页面上显示出来密码错误提示
        ui->stackedWidget_statusLogin->setCurrentWidget(ui->page_passwordError);
    }
    else
    {
        // 其他原因引起的登录失败，在页面显示出登录错误提示
        ui->stackedWidget_statusLogin->setCurrentWidget(ui->page_loginFailed);
    }
}

/*******************************************************************************
* Function Name  :  slot_modify_result
* Description    :  修改密码结果反馈
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetMain::slot_modify_result(int result)
{
    if(0 == result)
    {
        // 修改成功
        ui->stackedWidget_statusModify->setCurrentWidget(ui->page_modifySuccess);
    }
    else
    {
        // 修改失败
        ui->stackedWidget_statusModify->setCurrentWidget(ui->page_modifyFailed);
    }
}

/*******************************************************************************
* Function Name  :  on_button_passwordModify_clicked
* Description    :  修改密码按钮
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetMain::on_button_passwordModify_clicked()
{
    QString newPwd1 = ui->edit_newPassword1->text();
    QString newPwd2 = ui->edit_newPassword2->text();

    // 当两次输入的密码相同，且不为空时
    if(newPwd1 == newPwd2)
    {
        if(!newPwd1.isEmpty())
        {
            // 设置修改按钮可点击
            ui->button_passwordModify->setEnabled(true);
            // 正在修改的状态设置
            ui->stackedWidget_statusModify->setCurrentWidget(ui->page_modifing);
            // 发送修改密码的信号
            emit signal_user_modify(newPwd1);
        }
        else
        {
            ui->stackedWidget_statusModify->setCurrentWidget(ui->page_modifyNormal);
        }
    }
    else
    {
        // 显示当前两次输入密码不同
        ui->stackedWidget_statusModify->setCurrentWidget(ui->page_pwdNotSame);
    }
}

/*******************************************************************************
* Function Name  :  on_edit_newPassword2_editingFinished
* Description    :  密码输入完成
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetMain::on_edit_newPassword2_editingFinished()
{
    QString newPwd1 = ui->edit_newPassword1->text();
    QString newPwd2 = ui->edit_newPassword2->text();

    // 当两次输入的密码相同，且不为空时
    if(newPwd1 == newPwd2)
    {
        if(!newPwd1.isEmpty())
        {
            // 设置修改按钮可点击
            ui->button_passwordModify->setEnabled(true);
        }
        ui->stackedWidget_statusModify->setCurrentWidget(ui->page_modifyNormal);
    }
    else
    {
        // 显示当前两次输入密码不同
        ui->stackedWidget_statusModify->setCurrentWidget(ui->page_pwdNotSame);
    }
}

/*******************************************************************************
* Function Name  :  on_button_backModify_clicked
* Description    :  修改页面返回按钮点击
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetMain::on_button_backModify_clicked()
{
    ui->stackedWidget_main->setCurrentWidget(ui->page_content);

    // 初始化密码修改页面
    ui->edit_newPassword1->clear();
    ui->edit_newPassword2->clear();
    ui->stackedWidget_statusModify->setCurrentWidget(ui->page_modifyNormal);
    ui->button_passwordModify->setEnabled(false);
}

/*******************************************************************************
* Function Name  :  on_button_pageModify_clicked
* Description    :  跳转到修改密码页面
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetMain::on_button_pageModify_clicked()
{
    ui->stackedWidget_main->setCurrentWidget(ui->page_modify);
}

/*******************************************************************************
* Function Name  :  slot_quit_program
* Description    :  退出程序
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetMain::slot_quit_program()
{
    ui->stackedWidget_main->setCurrentWidget(ui->page_index);
}
