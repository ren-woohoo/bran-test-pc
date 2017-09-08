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
    debugInfo = "START TEST CPU ...\n";
    QString sn;
    QString cmd = " cat /proc/cpuinfo";
    qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";
    QString result = deviceItem->adb_shell(cmd);
    qDebug()<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    debugInfo.append(QString("CMD: %1\n").arg(cmd));
    debugInfo.append(QString("RESULT: %1\n").arg(result));
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
                debugInfo.append(QString("CURRENT SN:%1\n").arg(sn));
            }
        }
    }

    debugInfo.append(QString("SN:%1\ndeviceSN:%2\ndeviceADB:%3\n").arg(sn).arg(deviceSN).arg(deviceADB));

    if((sn == deviceSN) && (sn.contains(deviceADB.mid(8,7))))
    {
        debugInfo.append("END TEST CPU --- PASS!!!");
        emit signal_test_result(0, debugInfo);
    }
    else
    {
        debugInfo.append("END TEST CPU --- FAIL!!!");
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



