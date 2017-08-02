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
    debugInfo = "START TEST MIIO ...\n";
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
        debugInfo.append("MIIO INFO IS EMPTY!!!\n");
        debugInfo.append("END MIIO DEVICE CONF --- FAIL!!!");
        emit signal_test_result(-1, debugInfo);
        return;
    }
    debugInfo.append(QString("DID:  %1\n").arg(infoMIIO.did));
    debugInfo.append(QString("MAC:  %1\n").arg(infoMIIO.mac));
    debugInfo.append(QString("KEY:  %1\n").arg(infoMIIO.key));

    if((infoMIIO.did.length() == 8) && (infoMIIO.mac.length() == 17) && (infoMIIO.key.length() > 1))
    {
        QString cmd1 = QString("sed -i \"/did=/c did=%1\" %2").arg(infoMIIO.did).arg("/usr/bin/qtapp/etc/device.conf");
        QString result1 = deviceItem->excute_cmd(cmd1);
        debugInfo.append(QString("CMD1: %1\n").arg(cmd1));
        debugInfo.append(QString("RESULT1: %1\n").arg(result1));

        QString cmd2 = QString("sed -i \"/mac=/c mac=%1\" %2").arg(infoMIIO.mac).arg("/usr/bin/qtapp/etc/device.conf");
        QString result2 = deviceItem->excute_cmd(cmd2);
        debugInfo.append(QString("CMD2: %1\n").arg(cmd2));
        debugInfo.append(QString("RESULT2: %1\n").arg(result2));

        QString cmd3 = QString("sed -i \"/key=/c key=%1\" %2").arg(infoMIIO.key).arg("/usr/bin/qtapp/etc/device.conf");
        QString result3 = deviceItem->excute_cmd(cmd3);
        debugInfo.append(QString("CMD3: %1\n").arg(cmd3));
        debugInfo.append(QString("RESULT3: %1\n").arg(result3));

        QString cmd4 = "cat /usr/bin/qtapp/etc/device.conf";
        QString result4 = deviceItem->excute_cmd(cmd4);
        debugInfo.append(QString("CMD4: %1\n").arg(cmd4));
        debugInfo.append(QString("RESULT4: %1\n").arg(result4));
        QStringList listFile = result4.split("\n");
        QString did,mac,key;
        for(int i = 0; i < listFile.length(); ++i)
        {
            if(listFile.at(i).startsWith("did="))
            {
                did = listFile.at(i).split("=").at(1);
                did = did.trimmed();
            }
            else if(listFile.at(i).startsWith("mac="))
            {
                mac = listFile.at(i).split("=").at(1);
                mac = mac.trimmed();
            }
            else if(listFile.at(i).startsWith("key="))
            {
                key = listFile.at(i).split("=").at(1);
                key = key.trimmed();
            }
        }
        debugInfo.append(QString("DID1:\"%1\", DID2:\"%2\"\n").arg(infoMIIO.did).arg(did));
        debugInfo.append(QString("MAC1:\"%1\", MAC2:\"%2\"\n").arg(infoMIIO.mac).arg(mac));
        debugInfo.append(QString("KEY1:\"%1\", KEY3:\"%2\"\n").arg(infoMIIO.key).arg(key));
        if((did == infoMIIO.did) && (mac == infoMIIO.mac) && (key == infoMIIO.key))
        {
            debugInfo.append("END TEST MIIO --- PASS!!!");
            emit signal_update_infoMIIO(infoMIIO);
            emit signal_test_result(0, debugInfo);
        }
        else
        {
            debugInfo.append("END TEST MIIO --- FAIL!!!");
            emit signal_test_result(-1, debugInfo);
        }
    }
    else
    {
        debugInfo.append("MIIO INFO IS ABNORMAL!!!");
        debugInfo.append("END TEST MIIO --- FAIL!!!");
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
    debugInfo.append("GET MIIO INFO FAILED!!!\n");
    debugInfo.append(QString("SERVER REPLY: %1\n").arg(replyData));
    debugInfo.append("END MIIO DEVICE CONF --- FAIL!!!");
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
