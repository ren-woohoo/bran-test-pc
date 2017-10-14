#include "deviceControl.h"

DeviceControl *DeviceControl::instance = 0;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DeviceControl *DeviceControl::getInstance()
{
    if(0 == instance)
    {
        instance = new DeviceControl();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  DeviceControl
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DeviceControl::DeviceControl()
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
void DeviceControl::data_init()
{
    timerRefresh = new QTimer();
    timerRefresh->moveToThread(this);
    timerRefresh->setInterval(1000);
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DeviceControl::connect_init()
{
    connect(timerRefresh, SIGNAL(timeout()), this, SLOT(slot_update_devices()));
}

/*******************************************************************************
* Function Name  :  run
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DeviceControl::run()
{
    timerRefresh->start();
    this->exec();
}

/*******************************************************************************
* Function Name  :  slot_update_devices
* Description    :  刷新设备列表
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DeviceControl::slot_update_devices()
{
    QList<QString> listDevicesCurrent;
    QList<QString> listDevicesAdd;
    QList<QString> listDevicesDelete;
    QString strDevice;
    QStringList listDevice;
    QString sn;
    QProcess *process = new QProcess;
    QString cmd = QString("bin\\adb\\adb devices");
    process->start("cmd", QStringList()<<"/c"<<cmd);
    process->waitForStarted();
    if(process->waitForFinished())
    {
        QString result = QString::fromLocal8Bit(process->readAllStandardOutput());
        if(process)
        {
            process->close();
            delete process;
            process = 0;
        }
        if(result.contains("List of devices attached"))
        {
            QStringList strList = result.split("\r\n");
            strList.pop_front();
            for(int i = 0; i < strList.length(); ++i)
            {
                strDevice = strList.at(i);
                listDevice = strDevice.split("\t");
                sn = listDevice.at(0);
                if(!(sn.trimmed()).isEmpty())
                {
                    listDevicesCurrent.append(sn);
                }
            }
            if(listDevices == listDevicesCurrent)
            {
                return;
            }
            else if(listDevices.isEmpty() && !listDevicesCurrent.isEmpty())
            {
                listDevicesAdd = listDevicesCurrent;
            }
            else if(!listDevices.isEmpty() && listDevicesCurrent.isEmpty())
            {
                listDevicesDelete = listDevices;
            }
            else
            {
                for(int i = 0; i < listDevicesCurrent.length(); ++i)
                {
                    for(int j = 0; j < listDevices.length(); ++j)
                    {
                        if(listDevicesCurrent.at(i) == listDevices.at(j))
                        {
                            break;
                        }
                        if(j == (listDevices.length() - 1))
                        {
                            // 假如需要更新的
                            listDevicesAdd.append(listDevicesCurrent.at(i));
                        }
                    }
                }
                for(int m = 0; m < listDevices.length(); ++m)
                {
                    for(int n = 0; n < listDevicesCurrent.length(); ++n)
                    {
                        if(listDevicesCurrent.at(n) == listDevices.at(m))
                        {
                            break;
                        }
                        if(n == (listDevicesCurrent.length() - 1))
                        {
                            // 假如需要更新的
                            listDevicesDelete.append(listDevices.at(m));
                        }
                    }
                }
            }

            // 若有新的串口增加
            if(!listDevicesAdd.isEmpty())
            {
                emit signal_add_devices(listDevicesAdd);
            }

            // 若有新的串口减少
            if(!listDevicesDelete.isEmpty())
            {
                emit signal_delete_devices(listDevicesDelete);
            }
            listDevices = listDevicesCurrent;
        }
    }
}
