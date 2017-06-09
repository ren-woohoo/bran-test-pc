#include "testUSB.h"

/*******************************************************************************
* Function Name  :  TestUSB
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
TestUSB::TestUSB(DeviceItem *deviceItem, SerialItem *serialItem)
    : BaseTestItem(deviceItem, serialItem)
{
    loop = new QEventLoop(this);
    connect(serialItem, SIGNAL(signal_usbA0B1_feedback(QString)), this, SLOT(slot_usbA0B1_feedback(QString)));
    connect(serialItem, SIGNAL(signal_usbA1B0_feedback(QString)), this, SLOT(slot_usbA1B0_feedback(QString)));
    connect(serialItem, SIGNAL(signal_usbA1B1_feedback(QString)), this, SLOT(slot_usbA1B1_feedback(QString)));
}

/*******************************************************************************
* Function Name  :  start_test
* Description    :  开始检测
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestUSB::start_test()
{
    serialItem->write_data(CHANGE_USB_A0B1);
}

/*******************************************************************************
* Function Name  :  slot_usbA0B1_feedback
* Description    :  A0B1串口返回
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestUSB::slot_usbA0B1_feedback(QString replyData)
{
    // 改变成功
    if(replyData.contains("Success"))
    {
        debugInfo.append("A0B1 SUCCESS:\n");
        timeKeep.restart();
        while(timeKeep.elapsed() < 10000)
        {
            if(!is_charging_ac() && is_charging_usb())
            {
                debugInfo.append("Change Usb A1B1\n");
                serialItem->write_data(CHANGE_USB_A1B1);
                return;
            }
            QTimer::singleShot(1000, loop, SLOT(quit()));
            loop->exec();
        }
        emit signal_test_result(-1, debugInfo);
    }
    else
    {
        debugInfo.append("END TEST USB_VBUS ------ FAILED!!!");
        emit signal_test_result(-1, debugInfo);
    }
}

/*******************************************************************************
* Function Name  :  slot_usbA1B1_feedback
* Description    :  A1B1串口返回
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestUSB::slot_usbA1B1_feedback(QString replyData)
{
    // 改变成功
    if(replyData.contains("Success"))
    {
        debugInfo.append("A1B1 SUCCESS:\n");
        timeKeep.restart();
        while(timeKeep.elapsed() < 10000)
        {
            if(is_charging_ac() && is_charging_usb())
            {
                debugInfo.append("Change Usb A1B0\n");
                serialItem->write_data(CHANGE_USB_A1B0);
                return;
            }
        }
        emit signal_test_result(-1, debugInfo);
    }
    else
    {
        debugInfo.append("END TEST USB_VBUS ------ FAILED!!!");
        emit signal_test_result(-1, debugInfo);
    }
}

/*******************************************************************************
* Function Name  :  slot_usbA1B0_feedback
* Description    :  A1B0串口返回
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestUSB::slot_usbA1B0_feedback(QString replyData)
{
    // 改变成功
    if(replyData.contains("Success"))
    {
        debugInfo.append("A1B0 SUCCESS:\n");
        timeKeep.restart();
        while(timeKeep.elapsed() < 10000)
        {
            if(is_charging_ac() && !is_charging_usb())
            {
                debugInfo.append("Change Usb A1B0\n");
                emit signal_test_result(0, debugInfo);
                debugInfo.append("END TEST USB_VBUS ------ SUCCESS!!!");
                return;
            }
        }
        emit signal_test_result(-1, debugInfo);
    }
    else
    {
        debugInfo.append("END TEST USB_VBUS ------ FAILED!!!");
        emit signal_test_result(-1, debugInfo);
    }
}

/*******************************************************************************
* Function Name  :  is_charging_ac
* Description    :  判断是否通过底座充电
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool TestUSB::is_charging_ac()
{
    QString result = deviceItem->excute_cmd(QString("shell cat %1").arg(DEVICE_POWER_AC));
    int ac = 0;
    ac = result.trimmed().toInt();
    qDebug()<<"AC:"<<result;
    debugInfo.append(QString("AC:%1\n").arg(ac));
    if(1 == ac)
    {
        return true;
    }
    return false;
}

/*******************************************************************************
* Function Name  :  is_charing_usb
* Description    :  判断是否通过usb充电
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool TestUSB::is_charging_usb()
{
    qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
    QString result = deviceItem->excute_cmd(QString("shell cat %1").arg(DEVICE_POWER_USB));
    int usb = 0;
    usb = result.trimmed().toInt();
    qDebug()<<"USB:"<<result;
    debugInfo.append(QString("USB:%1\n").arg(usb));
    if(1 == usb)
    {
        return true;
    }
    return false;
}
