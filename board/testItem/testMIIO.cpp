#include "testMIIO.h"

/*******************************************************************************
* Function Name  :  TestMIIO
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
TestMIIO::TestMIIO(DeviceItem *deviceItem, SerialItem *serialItem)
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
void TestMIIO::start_test()
{
    debugInfo = "START MIIO DEVICE CONF ...\n";
    emit signal_get_infoMIIO(deviceSN);
}

/*******************************************************************************
* Function Name  :  slot_getInfoMIIO_success
* Description    :  获取米家配置信息成功
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestMIIO::slot_getInfoMIIO_success(InfoMIIO infoMIIO)
{
    if(infoMIIO.isEmpty())
    {
        debugInfo.append("Get new MIIO info is NULL!!!\n");
        debugInfo.append("END MIIO DEVICE CONF ------ FAILED!!!");
        emit signal_test_result(-1, debugInfo);
        return;
    }

    debugInfo.append(QString("DID:%1\n").arg(infoMIIO.did));
    debugInfo.append(QString("MAC:%1\n").arg(infoMIIO.mac));
    debugInfo.append(QString("KEY:%1\n").arg(infoMIIO.key));

    QString cmd1 = QString("push %1").arg(COMMAND_DEVICE_1);
    debugInfo.append(QString("CMD1: %1\n").arg(cmd1));
    QString result1 = deviceItem->excute_cmd(cmd1);
    debugInfo.append(QString("RESULT1: %1\n").arg(result1));

    QString cmd2 = QString("shell %1").arg(COMMAND_DEVICE_2);
    debugInfo.append(QString("CMD2: %1\n").arg(cmd2));
    QString result2 = deviceItem->excute_cmd(cmd2);
    debugInfo.append(QString("RESULT2: %1\n").arg(result2));

    QString cmd3 = QString("shell %1 %2 %3 %4").arg(COMMAND_DEVICE_3).arg(infoMIIO.did).arg(infoMIIO.key).arg(infoMIIO.mac);
    debugInfo.append(QString("CMD3: %1\n").arg(cmd3));
    QString result3 = deviceItem->excute_cmd(cmd3);
    debugInfo.append(QString("RESULT3: %1\n").arg(result3));
    if(result3.contains("START THE SHELL FOR DEVICE INIT") && result3.contains("SET MIIO SUCCESS"))
    {
        // 同步测试数据
        debugInfo.append("END MIIO DEVICE CONF ------ SUCCESS!!!");
        emit signal_update_infoMIIO(infoMIIO);
        qDebug()<<"--------------------------------------------------------------";
        emit signal_test_result(0, debugInfo);
    }
    else
    {
        debugInfo.append("END MIIO DEVICE CONFIG ------ FAILED!!!");
        emit signal_test_result(-1, debugInfo);
    }
}

/*******************************************************************************
* Function Name  :  slot_getInfoMIIO_failed
* Description    :  获取米家配置信息失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestMIIO::slot_getInfoMIIO_failed(QString replyData)
{
    debugInfo.append(QString("replyData:%1\n").arg(replyData));
    debugInfo.append("Get new MIIO info failed!!!\n");
    debugInfo.append("END MIIO DEVICE CONF ------ FAILED!!!");
    emit signal_test_result(-1, debugInfo);
}

/*******************************************************************************
* Function Name  :  set_deviceSN
* Description    :  设置设备序列号
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestMIIO::set_deviceSN(QString deviceSN)
{
    this->deviceSN = deviceSN;
}
