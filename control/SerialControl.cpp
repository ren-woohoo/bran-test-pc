#include "serialControl.h"

SerialControl *SerialControl::instance = 0;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SerialControl *SerialControl::getInstance()
{
    if(0 == instance)
    {
        instance = new SerialControl();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  SerialControl
* Description    :  串口控制
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SerialControl::SerialControl(QObject *parent)
    : QThread()
{
    data_init();
    connect_init();
    moveToThread(this);
}

/*******************************************************************************
* Function Name  :  data_init
* Description    :  数据初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SerialControl::data_init()
{
    timerRefresh = new QTimer();
    timerRefresh->moveToThread(this);
    timerRefresh->setInterval(1000);
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SerialControl::connect_init()
{
    connect(timerRefresh, SIGNAL(timeout()), this, SLOT(slot_refresh_ports()));
}

/*******************************************************************************
* Function Name  :  run
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SerialControl::run()
{
    timerRefresh->start();
    this->exec();
}

/*******************************************************************************
* Function Name  :  slot_refresh_serial
* Description    :  刷新串口
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SerialControl::slot_refresh_ports()
{
    QList<QString> listPortCurrent;
    QList<QString> listPortAdd;
    QList<QString> listPortDelete;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        listPortCurrent.append(info.portName());
    }
    if(listPort == listPortCurrent)
    {
        return;
    }
    else if(listPort.isEmpty() && !listPortCurrent.isEmpty())
    {
        listPortAdd = listPortCurrent;
    }
    else if(!listPort.isEmpty() && listPortCurrent.isEmpty())
    {
        listPortDelete = listPort;
    }
    else
    {
        for(int i = 0; i < listPortCurrent.length(); ++i)
        {
            for(int j = 0; j < listPort.length(); ++j)
            {
                if(listPortCurrent.at(i) == listPort.at(j))
                {
                    break;
                }
                if(j == (listPort.length() - 1))
                {
                    // 假如需要更新的
                    listPortAdd.append(listPortCurrent.at(i));
                }
            }
        }
        for(int m = 0; m < listPort.length(); ++m)
        {
            for(int n = 0; n < listPortCurrent.length(); ++n)
            {
                if(listPortCurrent.at(n) == listPort.at(m))
                {
                    break;
                }
                if(n == (listPortCurrent.length() - 1))
                {
                    // 假如需要更新的
                    listPortDelete.append(listPort.at(m));
                }
            }
        }
    }
    // 若有新的串口增加
    if(!listPortAdd.isEmpty())
    {
        emit signal_add_ports(listPortAdd);
    }

    // 若有新的串口减少
    if(!listPortDelete.isEmpty())
    {
        emit signal_delete_ports(listPortDelete);
    }
    listPort = listPortCurrent;
}
