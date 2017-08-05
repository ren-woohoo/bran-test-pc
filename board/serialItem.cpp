#include "serialItem.h"

/*******************************************************************************
* Function Name  :  SerialItem
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SerialItem::SerialItem(QObject *parent)
    : QSerialPort(parent)
{
    data_init();
    connect_init();
    qDebug()<<package_cmd(GET_DEVICE);
}

/*******************************************************************************
* Function Name  :  data_init
* Description    :  数据初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SerialItem::data_init()
{
    this->setBaudRate(QSerialPort::Baud115200);
    this->setDataBits(QSerialPort::Data8);
    this->setParity(QSerialPort::NoParity);
    this->setStopBits(QSerialPort::OneStop);
    this->setFlowControl(QSerialPort::NoFlowControl);


    timerDelay = new QTimer(this);
    timerDelay->setSingleShot(true);
    timerDelay->setInterval(500);
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SerialItem::connect_init()
{
    connect(this, SIGNAL(readyRead()), this, SLOT(slot_recive_data()));
    connect(timerDelay, SIGNAL(timeout()), this, SLOT(slot_read_data()));
}


/*******************************************************************************
* Function Name  :  slot_write_data
* Description    :  写数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SerialItem::slot_write_data(QString cmd)
{
    QByteArray ba;
    ba.append(QByteArray::fromHex(cmd.toLatin1()));
    qDebug()<<"$$$$$$$$$$$$$"<<ba.toHex();
    this->write(ba);
}

/*******************************************************************************
* Function Name  :  package_cmd
* Description    :  封装命令
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString SerialItem::package_cmd(CommandType command, QString data)
{
    QByteArray ba;
    int i = 0;
    int sum = 0;
    int check1 = 0;
    int check2 = 0;
    ba.append("CG");
    QString cmd;
    if(CHANGE_USB_A0B0 == command)
    {
        cmd = "Change Usb A0B0";
    }
    else if(CHANGE_USB_A0B1 == command)
    {
        cmd = "Change Usb A0B1";
    }
    else if(CHANGE_USB_A1B0 == command)
    {
        cmd = "Change Usb A1B0";
    }
    else if(CHANGE_USB_A1B1 == command)
    {
        cmd = "Change Usb A1B1";
    }
    else if(GET_VOLTAGE == command)
    {
        cmd = "Get Voltage";
    }
    else if(GET_DEVICE == command)
    {
        cmd = "Get Device";
    }
    else if(GET_FIXTURE == command)
    {
        cmd = "Get Fixture";
    }
    ba.append(cmd.length());
    ba.append(cmd);
    ba.append(ceil(data.length()/2));
    ba.append(QByteArray::fromHex(data.toLatin1()));

    for(i= 0; i < ba.length(); ++i)
    {
        sum += ba.at(i)&0x000000FF ;
    }
    check1 = sum/256;
    check2 = sum%256;
    ba.append(check1);
    ba.append(check2);
    return QString(ba.toHex());
}

/*******************************************************************************
* Function Name  :  package_cmd
* Description    :  封装命令
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString SerialItem::package_cmd(QString cmd, QString data)
{
    QByteArray ba;
    int i = 0;
    int sum = 0;
    int check1 = 0;
    int check2 = 0;
    ba.append("CG");
    ba.append(cmd.length());
    ba.append(cmd);
    ba.append(ceil(data.length()/2));
    ba.append(QByteArray::fromHex(data.toLatin1()));

    for(i= 0; i < ba.length(); ++i)
    {
        sum += ba.at(i)&0x000000FF ;
    }
    check1 = sum/256;
    check2 = sum%256;
    ba.append(check1);
    ba.append(check2);
    return QString(ba.toHex());
}

/*******************************************************************************
* Function Name  :  analyse_reply
* Description    :  解析返回信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SerialItem::analyse_reply(QString replyData)
{
    qDebug()<<replyData;
    QByteArray ba;
    QString strReply = QString(QByteArray::fromHex(replyData.toLatin1()));
    int lenCMD = 0;
    int lenDATA = 0;
    QString cmd;
    QString data;
    QString strCmd;
    QByteArray match;
    char *cLen = NULL;
    int len = 0;
    if(strReply.contains("CG"))
    {
        match.append("CG");
        len = strReply.indexOf("CG");
        strReply = strReply.mid(len);
        strReply = strReply.mid(2);
        ba = QString(strReply.at(0)).toLatin1();
        cLen = ba.data();
        lenCMD = *cLen;
        match.append(lenCMD);
        if(strReply.length() > lenCMD)
        {
            strReply = strReply.mid(1);
            cmd = strReply.mid(0,lenCMD);
            match.append(cmd);
            strReply = strReply.mid(lenCMD);
            ba = QString(strReply.at(0)).toLatin1();
            cLen = ba.data();
            lenDATA = *cLen;
            match.append(lenDATA);
            len = replyData.indexOf(match.toHex()) + QString(match.toHex()).length();
            data = replyData.mid(len,lenDATA*2);
            strCmd = package_cmd(cmd,data);
            if(replyData.contains(strCmd))
            {
                if("Change Usb A0B0" == cmd)
                {
                    emit signal_usbA0B0_feedback(replyData, data);
                }
                else if("Change Usb A0B1" == cmd)
                {
                    emit signal_usbA0B1_feedback(replyData, data);
                }
                else if("Change Usb A1B0" == cmd)
                {
                    emit signal_usbA1B0_feedback(replyData, data);
                }
                else if("Change Usb A1B1" == cmd)
                {
                    emit signal_usbA1B1_feedback(replyData, data);
                }
                else if("Get Voltage" == cmd)
                {
                    emit signal_getVoltage_feedback(replyData, data);
                }
                else if("Get Device" == cmd)
                {
                    emit signal_getDevice_feedback(replyData, data);
                }
                else if("Get Fixture" == cmd)
                {
                    emit signal_getFixture_feedback(replyData, data);
                }
            }
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_open_port
* Description    :  打开串口
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SerialItem::slot_open_port(QString port)
{
    this->port = port;
    this->setPortName(port);
    this->close();
    if(this->open(QIODevice::ReadWrite))
    {
        emit signal_openPort_result(0);
    }
    else
    {
        emit signal_openPort_result(-1);
    }
}

/*******************************************************************************
* Function Name  :  slot_close_port
* Description    :  关闭串口
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SerialItem::slot_close_port()
{
    this->close();
}

void SerialItem::slot_read_data()
{
    analyse_reply(dataCache.toHex());
    dataCache.clear();
}

/*******************************************************************************
* Function Name  :  slot_recive_data
* Description    :  接收到数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SerialItem::slot_recive_data()
{
    if(!timerDelay->isActive())
    {
        timerDelay->start();
    }

    // 读取新的数据
    dataCache.append(this->readAll());
    qDebug()<<"Cache:"<<QString(dataCache);
}
