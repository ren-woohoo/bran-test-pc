#include "testWiFi.h"

/*******************************************************************************
* Function Name  :  TestWiFi
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
TestWiFi::TestWiFi(DeviceItem *deviceItem, SerialItem *serialItem)
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
void TestWiFi::start_test()
{
    debugInfo = "START TEST WI-FI ...\n";
    QString cmd1 = "ps | grep wpa_supplicant | grep -v grep | wc -l";
    QString result1 = deviceItem->excute_cmd(cmd1);
    debugInfo.append(QString("CMD1: %1\n").arg(cmd1));
    debugInfo.append(QString("RESULT1: %1\n").arg(result1));
    if(!result1.isEmpty() && "0" == result1)
    {
        QString cmd2 = "ifconfig wlan0 up";
        QString result2 = deviceItem->excute_cmd(cmd2);
        debugInfo.append(QString("CMD2: %1\n").arg(cmd2));
        debugInfo.append(QString("RESULT2: %1\n").arg(result2));
        QString cmd3 = "echo ctrl_interface=/var/run/wpa_supplicant > /etc/wpa_supplicant.conf";
        QString result3 = deviceItem->excute_cmd(cmd3);
        debugInfo.append(QString("CMD3: %1\n").arg(cmd3));
        debugInfo.append(QString("RESULT3: %1\n").arg(result3));

        QString cmd4 = "wpa_supplicant -B -i wlan0 -D nl80211 -c /etc/wpa_supplicant.conf";
        QString result4 = deviceItem->excute_cmd(cmd4);
        debugInfo.append(QString("CMD4: %1\n").arg(cmd4));
        debugInfo.append(QString("RESULT4: %1\n").arg(result4));
    }

    QString cmd5 = "wpa_cli -i wlan0 scan";
    QString result5 = deviceItem->excute_cmd(cmd5);
    debugInfo.append(QString("CMD5: %1\n").arg(cmd5));
    debugInfo.append(QString("RESULT5: %1\n").arg(result5));

    // 做出2S延迟
    QEventLoop loop;
    QTimer::singleShot(2000, &loop, SLOT(quit()));
    loop.exec();

    QString cmd6 = "wpa_cli -i wlan0 scan_result";
    QString result6 = deviceItem->excute_cmd(cmd6);
    debugInfo.append(QString("CMD6: %1\n").arg(cmd6));
    debugInfo.append(QString("RESULT6: %1\n").arg(result6));

    QStringList strList = result6.trimmed().split("\n");
    if(strList.length() <= 1)
    {
        debugInfo.append("END TEST Wi-Fi --- FAIL!!!");
        emit signal_test_result(-1, debugInfo);
        return;
    }
    for(int i = 0; i < strList.length(); ++i)
    {
        if(i == 0)
        {
            continue;
        }
        QStringList list = strList.at(i).split("\t");
        Wifi wifi;
        wifi.name   = list[4].trimmed();
        wifi.value  = list[2].trimmed().toInt();
        debugInfo.append(QString("Wi-Fi%1:\" name: %2,strength: %3\"\n").arg(i).arg(wifi.name).arg(wifi.value));
    }
    debugInfo.append("END TEST Wi-Fi --- PASS!!!");
    emit signal_test_result(0, debugInfo);



//    QString cmd1 = QString("push %1").arg(WIFI_COMMAND1);
//    debugInfo.append(QString("CMD1: %1\n").arg(cmd1));
//    QString result1 = deviceItem->excute_cmd(cmd1);
//    debugInfo.append(QString("RESULT1: %1\n").arg(result1));
//    QString cmd2 = QString("shell %1").arg(WIFI_COMMAND2);
//    debugInfo.append(QString("CMD2: %1\n").arg(cmd2));
//    QString result2 = deviceItem->excute_cmd(cmd2);
//    debugInfo.append(QString("RESULT2: %1\n").arg(result2));
//    QString cmd3 = QString("shell %1").arg(WIFI_COMMAND3);
//    debugInfo.append(QString("CMD3: %1\n").arg(cmd3));
//    QString result3 = deviceItem->excute_cmd(cmd3);
//    debugInfo.append(QString("RESULT3: %1\n").arg(result3));

//    // 做出2S延迟
//    QEventLoop loop;
//    QTimer::singleShot(2000, &loop, SLOT(quit()));
//    loop.exec();

//    QString cmd4 = QString("shell %1").arg(WIFI_COMMAND4);
//    debugInfo.append(QString("CMD4: %1\n").arg(cmd4));
//    QString result4 = deviceItem->excute_cmd(cmd4);
//    qDebug()<<result1<<result2<<result3<<result4;
//    QStringList strList = result4.trimmed().split("\n");
//    if(strList.length() <= 1)
//    {
//        debugInfo.append(QString("RESULT4: %1\n").arg(result4));
//        debugInfo.append("END TEST Wi-Fi ------ FAILED!!!");
//        emit signal_test_result(-1, debugInfo);
//        return;
//    }
//    for(int i = 0; i < strList.length(); ++i)
//    {
//        if(i == 0)
//        {
//            continue;
//        }
//        QStringList list = strList.at(i).split("\t");
//        Wifi wifi;
//        wifi.name   = list[4].trimmed();
//        wifi.value  = list[2].trimmed().toInt();
//        debugInfo.append(QString("Wi-Fi%1:\" name: %2,strength: %3\"\n").arg(i).arg(wifi.name).arg(wifi.value));
//    }
//    debugInfo.append("END TEST Wi-Fi ------ SUCCESS!!!");
//    emit signal_test_result(0, debugInfo);
}
