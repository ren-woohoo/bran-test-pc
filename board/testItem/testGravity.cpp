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
    debugInfo = "START TEST G-SENSOR ...\n";
    QString cmd = QString("cat %1").arg(DEVICE_GRAVITY);
    QString result = deviceItem->adb_shell(cmd);
    debugInfo.append(QString("CMD: %1\n").arg(cmd));
    debugInfo.append(QString("RESULT: %1\n").arg(result));
    QStringList listResult = result.split(";");
    QStringList listStr1;
    QStringList listStr2;
    if(listResult.length() >= 2)
    {
        listStr1 = (listResult.at(0)).split("=");
        listStr2 = (listResult.at(1)).split("=");
        if((listStr1.length() == 2) && (listStr2.length() == 2))
        {
            int x = listStr1.at(1).trimmed().toInt();
            int y = listStr2.at(1).trimmed().toInt();
            debugInfo.append(QString("TEST x = %1, y = %2\n").arg(x).arg(y));
            debugInfo.append("END TEST G-SENSOR --- PASS!!!");
            emit signal_test_result(0, debugInfo);
            return;
        }
    }
    debugInfo.append("END TEST G-SENSOR --- FAIL!!!");
    emit signal_test_result(-1, debugInfo);
}
