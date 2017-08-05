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
    testItem1 = new TestControl(ui->widget_1);
    testItem2 = new TestControl(ui->widget_2);
    testItem3 = new TestControl(ui->widget_3);
    testItem4 = new TestControl(ui->widget_4);
    testItem5 = new TestControl(ui->widget_5);
    testItem6 = new TestControl(ui->widget_6);

    listTestItem.append(testItem1);
    listTestItem.append(testItem2);
    listTestItem.append(testItem3);
    listTestItem.append(testItem4);
    listTestItem.append(testItem5);
    listTestItem.append(testItem6);

    testItem1->start();
    testItem2->start();
    testItem3->start();
    testItem4->start();
    testItem5->start();
    testItem6->start();

    timerRefresh = new QTimer();
    timerRefresh->setSingleShot(true);
    timerRefresh->setInterval(3000);

    ui->button_start->setEnabled(false);
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
    connect(testItem1, SIGNAL(signal_test_end()), this, SLOT(slot_test_end()));
    connect(testItem2, SIGNAL(signal_test_end()), this, SLOT(slot_test_end()));
    connect(testItem3, SIGNAL(signal_test_end()), this, SLOT(slot_test_end()));
    connect(testItem4, SIGNAL(signal_test_end()), this, SLOT(slot_test_end()));
    connect(testItem5, SIGNAL(signal_test_end()), this, SLOT(slot_test_end()));
    connect(testItem6, SIGNAL(signal_test_end()), this, SLOT(slot_test_end()));

    connect(this, SIGNAL(signal_start_test()), testItem1, SLOT(slot_start_test()));
    connect(this, SIGNAL(signal_start_test()), testItem2, SLOT(slot_start_test()));
    connect(this, SIGNAL(signal_start_test()), testItem3, SLOT(slot_start_test()));
    connect(this, SIGNAL(signal_start_test()), testItem4, SLOT(slot_start_test()));
    connect(this, SIGNAL(signal_start_test()), testItem5, SLOT(slot_start_test()));
    connect(this, SIGNAL(signal_start_test()), testItem6, SLOT(slot_start_test()));

    connect(this, SIGNAL(signal_close_ports()), testItem1, SLOT(slot_close_port()), Qt::DirectConnection);
    connect(this, SIGNAL(signal_close_ports()), testItem2, SLOT(slot_close_port()), Qt::DirectConnection);
    connect(this, SIGNAL(signal_close_ports()), testItem3, SLOT(slot_close_port()), Qt::DirectConnection);
    connect(this, SIGNAL(signal_close_ports()), testItem4, SLOT(slot_close_port()), Qt::DirectConnection);
    connect(this, SIGNAL(signal_close_ports()), testItem5, SLOT(slot_close_port()), Qt::DirectConnection);
    connect(this, SIGNAL(signal_close_ports()), testItem6, SLOT(slot_close_port()), Qt::DirectConnection);

    connect(testItem1, SIGNAL(signal_openPort_failed(QString)), this, SLOT(slot_openPort_failed(QString)));
    connect(testItem2, SIGNAL(signal_openPort_failed(QString)), this, SLOT(slot_openPort_failed(QString)));
    connect(testItem3, SIGNAL(signal_openPort_failed(QString)), this, SLOT(slot_openPort_failed(QString)));
    connect(testItem4, SIGNAL(signal_openPort_failed(QString)), this, SLOT(slot_openPort_failed(QString)));
    connect(testItem5, SIGNAL(signal_openPort_failed(QString)), this, SLOT(slot_openPort_failed(QString)));
    connect(testItem6, SIGNAL(signal_openPort_failed(QString)), this, SLOT(slot_openPort_failed(QString)));

    connect(timerRefresh, SIGNAL(timeout()), this, SLOT(slot_refresh_cache()));
}

/*******************************************************************************
* Function Name  :  slot_show_error
* Description    :  显示错误信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetControl::slot_openPort_failed(QString port)
{
    QMessageBox::critical(this, tr("ERROR"), QString("串口%1,打开失败").arg(port), QMessageBox::Abort | QMessageBox::Ignore);
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
    for(int j = 0; j < listPorts.length(); ++j)
    {
        if(!listPortsCache.contains(listPorts.at(j)))
        {
            listPortsCache.append(listPorts.at(j));
        }
    }
    slot_refresh_cache();
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
    for(int j = 0; j < listPorts.length(); ++j)
    {
        if(!listPortsCache.contains(listPorts.at(j)))
        {
            listPortsCache.removeOne(listPorts.at(j));
        }
    }

    // 循环此时的检测项目
    for(int i = 0; i < listTestItem.length();++i)
    {
        // 若检测项目已安置检测项
        if(IS_FREE != listTestItem.at(i)->testStage)
        {
            // 循环此时移除的串口列表
            for(int j = 0; j < listPorts.length();)
            {
                // 若检测项目的串口已被移除
                if(listTestItem.at(i)->get_port() == listPorts.at(j))
                {
                    // 重置检测项
                    listTestItem.at(i)->remove_port();
                    listPorts.removeAt(i);
                    ui->button_start->setEnabled(false);
                    for(int m = 0; m < listTestItem.length(); ++m)
                    {
                        if(IS_MATCHED == listTestItem.at(m)->testStage)
                        {
                            ui->button_start->setEnabled(true);
                        }
                    }
                    for(int n = 0; n < listTestItem.length(); ++n)
                    {
                        if(IS_TESTING == listTestItem.at(n)->testStage)
                        {
                            ui->button_start->setEnabled(false);
                        }
                    }

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
                else
                {
                    ++j;
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
        }
    }
    slot_refresh_cache();
}

/*******************************************************************************
* Function Name  :  slot_delete_devices
* Description    :  移除设备
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetControl::slot_delete_devices(QList<QString> listDevices)
{
    for(int i = 0; i < listDevices.length();)
    {
        if(listDevicesCache.contains(listDevices.at(i)))
        {
            listDevicesCache.removeOne(listDevices.at(i));
        }
        else
        {
            ++i;
        }
    }
    for(int i = 0; i < listTestItem.length(); ++i)
    {
        if((IS_MATCHED == listTestItem.at(i)->testStage) || (IS_TESTING == listTestItem.at(i)->testStage))
        {
            for(int j = 0; j < listDevices.length();)
            {
                if(listTestItem.at(i)->get_device() == listDevices.at(j))
                {
                    // 移除设备
                    listTestItem.at(i)->remove_device();
                    ui->button_start->setEnabled(false);
                    for(int m = 0; m < listTestItem.length(); ++m)
                    {
                        if(IS_MATCHED == listTestItem.at(m)->testStage)
                        {
                            ui->button_start->setEnabled(true);
                        }
                    }
                    for(int n = 0; n < listTestItem.length(); ++n)
                    {
                        if(IS_TESTING == listTestItem.at(n)->testStage)
                        {
                            ui->button_start->setEnabled(false);
                        }
                    }
                    listDevices.removeAt(j);
                }
                else
                {
                    ++j;
                }
            }
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_refresh_cache
* Description    :  刷新缓存
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetControl::slot_refresh_cache()
{
    if(!listPortsCache.isEmpty())
    {
        for(int i = 0; i < listTestItem.length(); ++i)
        {
            // 若此时的检测项是空闲的
            if(IS_FREE == listTestItem.at(i)->testStage)
            {
                // 取出一个串口安置进去
                listTestItem.at(i)->load_port(listPortsCache.at(0));
                listPortsCache.pop_front();

                // 若可用没有串口了，结束循环，退出
                if(listPortsCache.isEmpty())
                {
                    break;
                }
            }
        }
    }
    if(!listDevicesCache.isEmpty())
    {
        for(int i = 0; i < listTestItem.length(); ++i)
        {
            if(IS_DEVICE_READY == listTestItem.at(i)->testStage)
            {
                for(int j = 0; j < listDevicesCache.length(); ++j)
                {
                    if(listTestItem.at(i)->get_deviceSN().contains(listDevicesCache.at(j).mid(8,7)))
                    {
                        listTestItem.at(i)->load_deviceADB(listDevicesCache.at(j));
                        ui->button_start->setEnabled(true);
                        for(int m = 0; m < listTestItem.length(); ++m)
                        {
                            if(IS_TESTING == listTestItem.at(m)->testStage)
                            {
                                ui->button_start->setEnabled(false);
                            }
                        }
                        listDevicesCache.removeAt(j);
                        break;
                    }
                }
            }
            else if(IS_FIXTURE_READY == listTestItem.at(i)->testStage)
            {
                if(!listDevicesCache.isEmpty())
                {
                    listTestItem.at(i)->sampling_deviceSN();
                }
            }
        }
    }

    if(listDevicesCache.isEmpty() && listPortsCache.isEmpty())
    {
        timerRefresh->stop();
    }
    else
    {
        timerRefresh->start();
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
    emit signal_start_test();
    ui->button_start->setEnabled(false);
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
        if(listTestItem.at(i)->testStage == IS_TESTING)
        {
            return;
        }
    }
    ui->button_start->setEnabled(true);
}
