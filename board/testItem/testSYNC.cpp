#include "testSYNC.h"

/*******************************************************************************
* Function Name  :  TestMIIO
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
TestSYNC::TestSYNC(DeviceItem *deviceItem, SerialItem *serialItem)
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
void TestSYNC::start_test(InfoTest infoTest)
{
    debugInfo = "START SYNC TEST RESULT ...\n";
    if(infoTest.infoResult.isPassed())
    {
        debugInfo.append("SYNC TEST-PASSED ...\n");
        if(infoTest.infoMIIO.isEmpty())
        {
            debugInfo.append("MIIO INFO IS EMPTY!!!\n");
            debugInfo.append("END SYNC TEST RESULT --- FAIL!!!");
            emit signal_test_result(-1, debugInfo);
            return;
        }
        emit signal_sync_testPassed(infoTest);
    }
    else
    {
        debugInfo.append("SYNC TEST-FAILED ...");
        emit signal_sync_testFailed(infoTest);
    }
}

/*******************************************************************************
* Function Name  :  slot_syncTest_result
* Description    :  同步测试结果
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestSYNC::slot_syncTest_success(QString requestData, QString replyData)
{
    debugInfo.append(QString("REQ: %1\n").arg(requestData));
    debugInfo.append(QString("ACK: %1\n").arg(replyData));
    QString cmd1 = "echo 1 > /usr/bin/qtapp/etc/board_test_passed";
    QString result1 = deviceItem->excute_cmd(cmd1);
    debugInfo.append(QString("CMD1: %1\n").arg(cmd1));
    debugInfo.append(QString("RESULT1: %1\n").arg(result1));
    QString cmd2 = "cat /usr/bin/qtapp/etc/board_test_passed";
    QString result2 = deviceItem->excute_cmd(cmd2);
    debugInfo.append(QString("CMD2: %1\n").arg(cmd2));
    debugInfo.append(QString("RESULT2: %1\n").arg(result2));
    if(result2.toInt() == 1)
    {
        debugInfo.append("END SYNC TEST RESULT --- PASS!!!");
        emit signal_test_result(0, debugInfo);
    }
    else
    {
        debugInfo.append("END SYNC TEST RESULT --- FAIL!!!");
        emit signal_test_result(-1, debugInfo);
    }
}

/*******************************************************************************
* Function Name  :  slot_syncTest_failed
* Description    :  同步测试错误
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestSYNC::slot_syncTest_failed(QString requestData, QString replyData)
{
    debugInfo.append(QString("REQ: %1\n").arg(requestData));
    debugInfo.append(QString("ACK: %1\n").arg(replyData));
    debugInfo.append("END SYNC TEST RESULT --- FAIL!!!");
    emit signal_test_result(-1,debugInfo);
}
