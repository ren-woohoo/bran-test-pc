#include "testGravity.h"

/*******************************************************************************
* Function Name  :  TestGravity
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
TestGravity::TestGravity(DeviceItem* deviceItem, SerialItem *serialItem)
    : BaseTestItem(deviceItem, serialItem)
{

}

/*******************************************************************************
* Function Name  :  start_test
* Description    :  开始检测
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestGravity::start_test()
{
    debugInfo = "START TEST G-SENSOR...\n";
    QString cmd = QString("shell cat %1").arg(DEVICE_GRAVITY);
    QString result = deviceItem->excute_cmd(cmd);
    debugInfo.append(QString("CMD: %1\n").arg(cmd));
    if((!result.contains(";")) || (!result.contains("=")))
    {
        debugInfo.append("END TEST G-SENSOR ------ FAILED!!!");
        emit signal_test_result(-1, debugInfo);
        return;
    }
    int x = (result.split(";").at(0)).split("=").at(1).trimmed().toInt();
    int y = (result.split(";").at(1)).split("=").at(1).trimmed().toInt();
    debugInfo.append(QString("TEST x = %1, y = %2\n").arg(x).arg(y));
    debugInfo.append("END TEST G-SENSOR ------ SUCCESS!!!");
    emit signal_test_result(0, debugInfo);
}
