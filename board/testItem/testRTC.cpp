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
    QString result = deviceItem->excute_cmd("shell hwclock");
    if(result.contains("error"))
    {
        debugInfo.append("END TEST CPU SERIAL NUMBER ------ FAILED!!!");
        emit signal_test_result(-1, debugInfo);
        return;
    }
    else
    {
        emit signal_test_result(0, debugInfo);
    }
}
