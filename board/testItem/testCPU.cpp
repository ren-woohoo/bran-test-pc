#include "testCPU.h"

/*******************************************************************************
* Function Name  :  TestCPU
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
TestCPU::TestCPU(DeviceItem* deviceItem, SerialItem *serialItem)
    : BaseTestItem(deviceItem, serialItem)
{

}

/*******************************************************************************
* Function Name  :  start_test
* Description    :  开始测试
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestCPU::start_test()
{
    QString sn;
    QString result = deviceItem->excute_cmd("shell cat /proc/cpuinfo");
    QStringList resultList = result.split("\n");
    for(int i = 0; i < resultList.length(); ++i)
    {
        debugInfo.append(QString("%1\n").arg(resultList.at(i).trimmed()));
        if((resultList.at(i)).contains("Chipid"))
        {
            QStringList strList = resultList.at(i).split(":");
            if(strList.length() == 2)
            {
                sn = strList.at(1);
                sn = sn.trimmed();
                debugInfo.append(QString("SN:%1\n").arg(sn));
            }
        }
    }
    qDebug()<<sn;
    if((sn == deviceSN) && (sn.contains(deviceADB.mid(8,7))))
    {
        emit signal_test_result(0, debugInfo);
    }
    else
    {
        emit signal_test_result(-1, debugInfo);
    }
}

/*******************************************************************************
* Function Name  :  set_deviceADB
* Description    :  设置设备adb值
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestCPU::set_deviceADB(QString deviceADB)
{
    this->deviceADB = deviceADB;
}

/*******************************************************************************
* Function Name  :  set_deviceSN
* Description    :  设置设备SN值
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestCPU::set_deviceSN(QString deviceSN)
{
    this->deviceSN = deviceSN;
}



