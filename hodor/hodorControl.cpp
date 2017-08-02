#include "hodorControl.h"

HodorControl *HodorControl::instance = 0;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
HodorControl *HodorControl::getInstance()
{
    if(0 == instance)
    {
        instance = new HodorControl();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  HodorControl
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
HodorControl::HodorControl()
    : QThread()
{
    qRegisterMetaType<InfoDevice>("InfoDevice");
    qRegisterMetaType<InfoHodor>("InfoHodor");
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
void HodorControl::data_init()
{
    isFree = true;
    deviceItem = new DeviceItem();
    deviceItem->moveToThread(this);
    timer = new QTimer();
    timer->moveToThread(this);
    timer->setInterval(1000);

    serverHodor = new ServerHodor();
    serverHodor->moveToThread(this);

    serverUser = ServerUser::getInstance();
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void HodorControl::connect_init()
{
    connect(serverUser, SIGNAL(signal_update_user(QString,QString)), serverHodor, SLOT(slot_update_user(QString,QString)));
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_refresh_device()));

    connect(this, SIGNAL(signal_sync_testPassed(InfoDevice,InfoHodor)), serverHodor, SLOT(slot_sync_testPassed(InfoDevice,InfoHodor)));
    connect(this, SIGNAL(signal_sync_testFailed(InfoDevice,InfoHodor)), serverHodor, SLOT(slot_sync_testFailed(InfoDevice,InfoHodor)));

    connect(serverHodor, SIGNAL(signal_syncTest_failed(QString, QString)),this, SIGNAL(signal_syncTest_failed(QString, QString)));
    connect(serverHodor, SIGNAL(signal_syncTest_success(QString, QString)), this, SIGNAL(signal_syncTest_success(QString, QString)));
}

/*******************************************************************************
* Function Name  :  run
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void HodorControl::run()
{
    timer->start();
    this->exec();
}

/*******************************************************************************
* Function Name  :  slot_add_devices
* Description    :  增加设备
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void HodorControl::slot_add_devices(QList<QString> listDevices)
{
    // 将新增加的设备都加入缓存设备列表中
    for(int j = 0; j < listDevices.length(); ++j)
    {
        if(!listDevicesCache.isEmpty())
        {
            if(!listDevicesCache.contains(listDevices.at(j)))
            {
                listDevicesCache.append(listDevices.at(j));
            }
        }
        else
        {
            qDebug()<<listDevices.length();
            listDevicesCache.append(listDevices.at(j));
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
void HodorControl::slot_delete_devices(QList<QString> listDevices)
{
    for(int j = 0; j < listDevices.length(); ++j)
    {
        if(listDevicesCache.contains(listDevices.at(j)))
        {
            listDevicesCache.removeOne(listDevices.at(j));
            if(!isFree)
            {
                if(listDevices.at(j) == infoDevice.deviceADB)
                {
                    emit signal_remove_device();
                    isFree = true;
                }
            }
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_refresh_device
* Description    :  刷新设备
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void HodorControl::slot_refresh_device()
{
    if(isFree)
    {
        if(listDevicesCache.length() > 0)
        {
            if(refresh_device(listDevicesCache.at(0)))
            {
                isFree = false;
            }
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_sync_hodor
* Description    :  同步hodor
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void HodorControl::slot_sync_hodor()
{
    if(!isFree)
    {
        if(infoHodor.isPassed())
        {
            emit signal_sync_testPassed(infoDevice, infoHodor);
        }
        else
        {
            emit signal_sync_testFailed(infoDevice, infoHodor);
        }
    }
}

/*******************************************************************************
* Function Name  :  refresh_device
* Description    :  刷新设备信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool HodorControl::refresh_device(QString deviceADB)
{
    infoDevice.clear();
    infoHodor.init();

    deviceItem->set_device(deviceADB);
    infoDevice.deviceADB = deviceADB;
    QString result1 = deviceItem->excute_cmd("cat /usr/bin/qtapp/hodor_result.txt");
    QStringList resultList1;
    QString item;
    QString result;
    if(result1.contains("TEST_"))
    {
        resultList1 = (result1.trimmed()).split("\n");
        if(resultList1.length() < 9)
        {
            return false;
        }
        for(int i = 0; i < resultList1.length(); ++i)
        {
            item = resultList1.at(i);
            item = item.trimmed();
            if(item.contains(TEST_BATTERY))
            {
                result = item.split("=").at(1);
                if(result == "success")
                {
                    infoHodor.battery = true;
                }
            }
            else if(item.contains(TEST_BUTTON))
            {
                result = item.split("=").at(1);
                if(result == "success")
                {
                    infoHodor.button = true;
                }
            }
            else if(item.contains(TEST_GRAVITY))
            {
                result = item.split("=").at(1);
                if(result == "success")
                {
                    infoHodor.gravity = true;
                }
            }
            else if(item.contains(TEST_LIGHT))
            {
                result = item.split("=").at(1);
                if(result == "success")
                {
                    infoHodor.light = true;
                }
            }
            else if(item.contains(TEST_PM25))
            {
                result = item.split("=").at(1);
                if(result == "success")
                {
                    infoHodor.pm25 = true;
                }
            }
            else if(item.contains(TEST_TEMP_HUMI))
            {
                result = item.split("=").at(1);
                if(result == "success")
                {
                    infoHodor.tempHumi = true;
                }
            }
            else if(item.contains(TEST_TOUCH))
            {
                result = item.split("=").at(1);
                if(result == "success")
                {
                    infoHodor.touch = true;
                }
            }
            else if(item.contains(TEST_TVOC_CO2E))
            {

                result = item.split("=").at(1);
                if(result == "success")
                {
                    infoHodor.tvocCo2e = true;
                }

            }
            else if(item.contains(TEST_WIFI))
            {
                result = item.split("=").at(1);
                if(result == "success")
                {
                    infoHodor.wifi = true;
                }
            }
        }
    }
    else
    {
        return false;
    }

    QString result2 = deviceItem->excute_cmd("cat /usr/bin/qtapp/etc/device.conf");
    QStringList resultList2 = result2.split("\n");
    QString row;
    if(resultList2.length() < 4)
    {
        return false;
    }
    for(int i = 0; i < resultList2.length(); ++i)
    {
        row = resultList2.at(i);
        row = row.trimmed();
        if(row.contains("did="))
        {
            infoDevice.infoMiio.did = row.split("=").at(1);
        }
        else if(row.contains("mac="))
        {
            infoDevice.infoMiio.mac = row.split("=").at(1);
        }
        else if(row.contains("key="))
        {
            infoDevice.infoMiio.key = row.split("=").at(1);
        }
    }
    if(infoDevice.infoMiio.isEmpty())
    {
        return false;
    }
    QString result3 = deviceItem->excute_cmd("cat /proc/cpuinfo");
    QStringList resultList3 = result3.split("\n");
    for(int i = 0; i < resultList3.length(); ++i)
    {
        if((resultList3.at(i)).contains("Chipid"))
        {
            QStringList strList = resultList3.at(i).split(":");
            if(strList.length() == 2)
            {
                infoDevice.deviceSN = strList.at(1);
                infoDevice.deviceSN = infoDevice.deviceSN.trimmed();
            }
        }
    }
    if(!infoDevice.isEmpty() && (infoDevice.deviceSN.contains(infoDevice.deviceADB.mid(8,7))))
    {
        emit signal_update_device(infoDevice, infoHodor);
    }
    else
    {
        return true;
    }
}

