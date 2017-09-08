#include "testRTC.h"

/*******************************************************************************
* Function Name  :  TestRTC
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
TestRTC::TestRTC(DeviceItem *deviceItem, SerialItem *serialItem)
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
void TestRTC::start_test()
{
    debugInfo = "START TEST RTC ...\n";
    QString cmd = "hwclock";
    QString result = deviceItem->adb_shell(cmd);
    debugInfo.append(QString("CMD: %1\n").arg(cmd));
    debugInfo.append(QString("RESULT: %1\n").arg(result));
    if(result.contains("seconds"))
    {
        debugInfo.append("END TEST RTC --- PASS!!!");
        emit signal_test_result(0, debugInfo);
        return;
    }
    debugInfo.append("END TEST RTC --- FAIL!!!");
    emit signal_test_result(-1, debugInfo);
}
