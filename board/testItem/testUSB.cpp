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
    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(5000);

    connect(timer, SIGNAL(timeout()), this, SLOT(slot_serial_timeout()));
    connect(serialItem, SIGNAL(signal_usbA0B1_feedback(QString,QString)), this, SLOT(slot_usbA0B1_feedback(QString,QString)));
    connect(serialItem, SIGNAL(signal_usbA1B0_feedback(QString,QString)), this, SLOT(slot_usbA1B0_feedback(QString,QString)));
    connect(serialItem, SIGNAL(signal_usbA1B1_feedback(QString,QString)), this, SLOT(slot_usbA1B1_feedback(QString,QString)));
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
    debugInfo = "START TEST DCIN_VBUS ...\n";
    cmd = serialItem->package_cmd(CHANGE_USB_A0B1);
    debugInfo.append(QString("SEND:%1, HEX:%2\n").arg(QString(QByteArray::fromHex(cmd.toLatin1()))).arg(cmd));
    emit signal_write_data(cmd);
    timer->start();
}

/*******************************************************************************
* Function Name  :  slot_usbA0B1_feedback
* Description    :  A0B1串口返回
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestUSB::slot_usbA0B1_feedback(QString replyData,QString data)
{
    debugInfo.append(QString("RECIVE:%1, DATA:%2, ASCII:%3\n").arg(replyData).arg(data).arg(QString(QByteArray::fromHex(data.toLatin1()))));
    if(serialItem->package_cmd(CHANGE_USB_A0B1) != cmd)
    {
        return;
    }
    timer->stop();
    data = QString(QByteArray::fromHex(data.toLatin1()));
    if("Success" == data)
    {
        debugInfo.append("VERIFY A0B1 ...\n");
        timeKeep.restart();
        while(timeKeep.elapsed() < 3000)
        {
            if(update_ac(ac) && update_usb(usb))
            {
                if(!ac && usb)
                {
                    debugInfo.append("CHANGE USB A1B1 ...\n");
                    cmd = serialItem->package_cmd(CHANGE_USB_A1B1);
                    debugInfo.append(QString("SEND:%1, HEX:%2\n").arg(QString(QByteArray::fromHex(cmd.toLatin1()))).arg(cmd));
                    emit signal_write_data(cmd);
                    timer->start();
                    return;
                }
            }
            QTimer::singleShot(500, loop, SLOT(quit()));
            loop->exec();
        }
        debugInfo.append("TIME OUT!!!\n");
        debugInfo.append("END TEST USB_VBUS --- FAIL!!!");
        emit signal_test_result(-1, debugInfo);
    }
    else
    {
        debugInfo.append("A0B1 IS FAILED!!!\n");
        debugInfo.append("END TEST USB_VBUS --- FAIL!!!");
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
void TestUSB::slot_usbA1B1_feedback(QString replyData,QString data)
{
    debugInfo.append(QString("RECIVE:%1, DATA:%2, ASCII:%3\n").arg(replyData).arg(data).arg(QString(QByteArray::fromHex(data.toLatin1()))));
    if(serialItem->package_cmd(CHANGE_USB_A1B1) != cmd)
    {
        return;
    }
    timer->stop();
    data = QString(QByteArray::fromHex(data.toLatin1()));
    if("Success" == data)
    {
        debugInfo.append("VERIFY A1B1 ...\n");
        timeKeep.restart();
        while(timeKeep.elapsed() < 3000)
        {
            if(update_ac(ac) && update_usb(usb))
            {
                if(ac && usb)
                {
                    debugInfo.append("CHANGE USB A1B0 ...\n");
                    cmd = serialItem->package_cmd(CHANGE_USB_A1B0);
                    debugInfo.append(QString("SEND:%1, HEX:%2\n").arg(QString(QByteArray::fromHex(cmd.toLatin1()))).arg(cmd));
                    emit signal_write_data(cmd);
                    timer->start();
                    return;
                }
            }
            QTimer::singleShot(500, loop, SLOT(quit()));
            loop->exec();
        }
        debugInfo.append("TIME OUT!!!\n");
        debugInfo.append("END TEST USB_VBUS --- FAIL!!!");
        emit signal_test_result(-1, debugInfo);
    }
    else
    {
        debugInfo.append("A1B1 IS FAILED!!!\n");
        debugInfo.append("END TEST USB_VBUS --- FAIL!!!");
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
void TestUSB::slot_usbA1B0_feedback(QString replyData,QString data)
{
    debugInfo.append(QString("RECIVE:%1, DATA:%2, ASCII:%3\n").arg(replyData).arg(data).arg(QString(QByteArray::fromHex(data.toLatin1()))));
    if(serialItem->package_cmd(CHANGE_USB_A1B0) != cmd)
    {
        return;
    }
    timer->stop();
    data = QString(QByteArray::fromHex(data.toLatin1()));

    // 改变成功
    if("Success" == data)
    {
        debugInfo.append("VERIFY A1B0 ...\n");
        timeKeep.restart();
        while(timeKeep.elapsed() < 3000)
        {
            if(update_ac(ac) && update_usb(usb))
            {
                if(ac && !usb)
                {
                    debugInfo.append("END TEST USB_VBUS --- PASS!!!");
                    emit signal_test_result(0, debugInfo);
                    return;
                }
            }
            QTimer::singleShot(500, loop, SLOT(quit()));
            loop->exec();
        }
        debugInfo.append("TIME OUT!!!\n");
        debugInfo.append("END TEST USB_VBUS --- FAIL!!!");
        emit signal_test_result(-1, debugInfo);
    }
    else
    {
        debugInfo.append("A1B0 IS FAILED!!!\n");
        debugInfo.append("END TEST USB_VBUS --- FAIL!!!");
        emit signal_test_result(-1, debugInfo);
    }
}

/*******************************************************************************
* Function Name  :  update_ac
* Description    :  判断是否通过底座充电
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool TestUSB::update_ac(bool &ac)
{
    QString cmd = "cat /sys/class/power_supply/ac/present";
    QString result = deviceItem->adb_shell(cmd);
    result = result.trimmed();
    debugInfo.append(QString("CMD: %1\n").arg(cmd));
    debugInfo.append(QString("RESULT: %1\n").arg(result));

    if(result.length() == 1)
    {
        if(result.toInt() == 1)
        {
            ac = true;
            return true;
        }
        else if(result.toInt() == 0)
        {
            ac = false;
            return true;
        }
    }
    debugInfo.append("UPDATE AC IS FAILED!!!\n");
    return false;
}

/*******************************************************************************
* Function Name  :  update_usb
* Description    :  判断是否通过usb充电
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool TestUSB::update_usb(bool &usb)
{
    QString cmd = "cat /sys/class/power_supply/usb/present";
    QString result = deviceItem->adb_shell(cmd);
    result = result.trimmed();
    debugInfo.append(QString("CMD: %1\n").arg(cmd));
    debugInfo.append(QString("RESULT: %1\n").arg(result));

    if(result.length() == 1)
    {
        if(result.toInt() == 1)
        {
            usb = true;
            return true;
        }
        else if(result.toInt() == 0)
        {
            usb = false;
            return true;
        }
    }
    debugInfo.append("UPDATE USB IS FAILED!!!\n");
    return false;
}

/*******************************************************************************
* Function Name  :  slot_serial_timeout
* Description    :  串口超时
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestUSB::slot_serial_timeout()
{
    debugInfo.append("SERIAL TIME OUT!!!\n");
    debugInfo.append("END TEST USB_VBUS --- FAIL!!!");
    emit signal_test_result(-1, debugInfo);
}
