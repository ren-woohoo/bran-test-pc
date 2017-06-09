#include "widgetControl.h"
#include "ui_widgetControl.h"

WidgetControl::WidgetControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetControl)
{
    qRegisterMetaType<QList<QString> > ("QList<QString>");
    ui->setupUi(this);
    data_init();
    connect_init();
}

WidgetControl::~WidgetControl()
{
    delete ui;
}

/*******************************************************************************
* Function Name  :  data_init()
* Description    :  数据初试化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetControl::data_init()
{
    widgetTestItem1 = new WidgetTestItem(ui->widget_1);
    widgetTestItem2 = new WidgetTestItem(ui->widget_2);
    widgetTestItem3 = new WidgetTestItem(ui->widget_3);
    widgetTestItem4 = new WidgetTestItem(ui->widget_4);
    widgetTestItem5 = new WidgetTestItem(ui->widget_5);
    widgetTestItem6 = new WidgetTestItem(ui->widget_6);

    listTestItem.append(widgetTestItem1);
    listTestItem.append(widgetTestItem2);
    listTestItem.append(widgetTestItem3);
    listTestItem.append(widgetTestItem4);
    listTestItem.append(widgetTestItem5);
    listTestItem.append(widgetTestItem6);

    timerVerify = new QTimer();
    timerVerify->setInterval(1000);
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetControl::connect_init()
{
    connect(widgetTestItem1, SIGNAL(signal_match_device(QString)), this, SLOT(slot_match_device(QString)));
    connect(widgetTestItem2, SIGNAL(signal_match_device(QString)), this, SLOT(slot_match_device(QString)));
    connect(widgetTestItem3, SIGNAL(signal_match_device(QString)), this, SLOT(slot_match_device(QString)));
    connect(widgetTestItem4, SIGNAL(signal_match_device(QString)), this, SLOT(slot_match_device(QString)));
    connect(widgetTestItem5, SIGNAL(signal_match_device(QString)), this, SLOT(slot_match_device(QString)));
    connect(widgetTestItem6, SIGNAL(signal_match_device(QString)), this, SLOT(slot_match_device(QString)));

    connect(widgetTestItem1, SIGNAL(signal_test_end()), this, SLOT(slot_test_end()));
    connect(widgetTestItem2, SIGNAL(signal_test_end()), this, SLOT(slot_test_end()));
    connect(widgetTestItem3, SIGNAL(signal_test_end()), this, SLOT(slot_test_end()));
    connect(widgetTestItem4, SIGNAL(signal_test_end()), this, SLOT(slot_test_end()));
    connect(widgetTestItem5, SIGNAL(signal_test_end()), this, SLOT(slot_test_end()));
    connect(widgetTestItem6, SIGNAL(signal_test_end()), this, SLOT(slot_test_end()));

    connect(timerVerify, SIGNAL(timeout()), this, SLOT(slot_verify_device()));
}

/*******************************************************************************
* Function Name  :  slot_test_end
* Description    :  检测结束
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetControl::slot_test_end()
{
    for(int i = 0; i < listTestItem.length(); ++i)
    {
        // 若此时还有检测项
        if(listTestItem.at(i)->get_status() == STAGE_TESTING)
        {
            return;
        }
    }
    ui->button_start->setEnabled(true);
}

/*******************************************************************************
* Function Name  :  slot_add_ports
* Description    :  增加治具
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetControl::slot_add_ports(QList<QString> listPorts)
{
    // 循环此时的检测项目组
    for(int i = 0; i < listTestItem.length(); ++i)
    {
        // 若此时的检测项是空闲的
        if(listTestItem.at(i)->get_status() == FREE)
        {
            // 取出一个串口安置进去
            listTestItem.at(i)->load_fixture(listPorts.at(0));
            listPorts.pop_front();

            // 若可用没有串口了，结束循环，退出
            if(listPorts.isEmpty())
            {
                return;
            }
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_delete_ports
* Description    :  移除串口
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetControl::slot_delete_ports(QList<QString> listPorts)
{
    // 循环此时的检测项目
    for(int i = 0; i < listTestItem.length(); ++i)
    {
        // 若检测项目已安置检测项
        if(listTestItem.at(i)->get_status() > FREE)
        {
            // 循环此时移除的串口列表
            for(int j = 0; j < listPorts.length(); ++j)
            {
                // 若检测项目的串口已被移除
                if(listTestItem.at(i)->get_fixture() == listPorts.at(j))
                {
                    // 重置检测项

                    listTestItem.at(i)->remove_fixture();
                    listPorts.removeAt(i);

                    // 若已没有其他移除项，返回，若还有，继续循环
                    if(listPorts.isEmpty())
                    {
                        return;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_add_devices
* Description    :  增加设备
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetControl::slot_add_devices(QList<QString> listDevices)
{
    // 将新增加的设备都加入缓存设备列表中
    for(int j = 0; j < listDevices.length(); ++j)
    {
        if(!listDevicesCache.contains(listDevices.at(j)))
        {
            listDevicesCache.append(listDevices.at(j));
            timerVerify->start();
        }
    }

    // 所有加载了治具的检测项且没有连接到设备的，检测一次设备做出匹配
    for(int i = 0; i < listTestItem.length(); ++i)
    {
        if((listTestItem.at(i)->get_status() == CONNECTED) || (listTestItem.at(i)->get_status() == READY))
        {
            // 检测一次设备
            listTestItem.at(i)->sample_device();
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_verify
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetControl::slot_verify_device()
{
    if(listDevicesCache.isEmpty())
    {
        timerVerify->stop();
        return;
    }

    // 所有加载了治具的检测项且没有连接到设备的，检测一次设备做出匹配
    for(int i = 0; i < listTestItem.length(); ++i)
    {
        if((listTestItem.at(i)->get_status() == CONNECTED) || (listTestItem.at(i)->get_status() == READY))
        {
            // 检测一次设备
            listTestItem.at(i)->sample_device();
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_delete_devices
* Description    :  删除设备
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetControl::slot_delete_devices(QList<QString> listDevices)
{
    for(int i = 0; i < listTestItem.length(); ++i)
    {
        qDebug()<<listTestItem.at(i)->get_status();
        if(listTestItem.at(i)->get_status() >= MATCHED)
        {
            for(int j = 0; j < listDevices.length(); ++j)
            {
                if(listTestItem.at(i)->get_device() == listDevices.at(j))
                {
                    // 移除设备
                    listTestItem.at(i)->remove_device();
                    listDevices.removeAt(j);
                }
            }
        }
    }
    for(int m = 0; m < listDevices.length(); ++m)
    {
        if(listDevicesCache.contains(listDevices.at(m)))
        {
            listDevicesCache.removeOne(listDevices.at(m));
            if(listDevicesCache.isEmpty())
            {
                timerVerify->stop();
            }
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_match_device
* Description    :  匹配设备
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetControl::slot_match_device(QString deviceSN)
{
    qDebug()<<deviceSN;
    QString sn;
    for(int i = 0; i < listDevicesCache.length(); ++i)
    {
        sn = listDevicesCache.at(i);
        sn = sn.mid(8,7);
        if(deviceSN.contains(sn))
        {
            for(int j = 0; j < listTestItem.length(); ++j)
            {
                qDebug()<<j<<listTestItem.at(j)->get_status();
                if(listTestItem.at(j)->get_status() == READY)
                {
                    if(listTestItem.at(j)->get_deviceSN() == deviceSN)
                    {
                        listTestItem.at(j)->load_device(listDevicesCache.at(i));
                        listDevicesCache.removeAt(i);
                        if(listDevicesCache.isEmpty())
                        {
                            timerVerify->stop();
                        }
                        return;
                    }
                }
            }
        }
    }
}

/*******************************************************************************
* Function Name  :  on_button_start_clicked
* Description    :  开始检测
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetControl::on_button_start_clicked()
{
    for(int i =0; i < listTestItem.length(); ++i)
    {
        // 若治具已做到匹配
        if((listTestItem.at(i)->get_status() == MATCHED) || (listTestItem.at(i)->get_status() == STAGE_TESTED))
        {
            // 则开始检测
            listTestItem.at(i)->start_test();
            ui->button_start->setEnabled(false);
        }
    }
}
