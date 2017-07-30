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
* Function Name  :  open_port
* Description    :  打开串口
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SerialItem::open_port(QString port)
{
    this->port = port;
    this->setPortName(port);
    this->close();
    if(this->open(QIODevice::ReadWrite))
    {
        return true;
    }
    return false;
}

/*******************************************************************************
* Function Name  :  close_port
* Description    :  关闭串口
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SerialItem::close_port()
{
    this->close();
}

void SerialItem::slot_read_data()
{
    QString replyData(dataCache);
    dataCache.clear();
    qDebug()<<replyData<<QThread::currentThreadId();
    if(replyData.contains("A0B0"))
    {
        emit signal_usbA0B0_feedback(replyData);
    }
    else if(replyData.contains("A0B1"))
    {
        emit signal_usbA0B1_feedback(replyData);
    }
    else if(replyData.contains("A1B0"))
    {
        emit signal_usbA1B0_feedback(replyData);
    }
    else if(replyData.contains("A1B1"))
    {
        emit signal_usbA1B1_feedback(replyData);
    }
    else if(replyData.contains("Voltage"))
    {
        emit signal_getVoltage_feedback(replyData);
    }
    else if(replyData.contains("Get Device"))
    {
        emit signal_getDevice_feedback(replyData);
    }
    else if(replyData.contains("Get Fixture"))
    {
        emit signal_getFixture_feedback(replyData);
    }
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


/*******************************************************************************
* Function Name  :  write_data
* Description    :  写数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SerialItem::write_data(CommandType command,int len, QString data)
{
    QByteArray ba;
    int i = 0;
    int sum = 0;
    int check1 = 0;
    int check2 = 0;
    ba.append("CG");

    if(CHANGE_USB_A0B0 == command)
    {
        ba.append(15);
        ba.append("Change Usb A0B0");
        ba.append(len);
    }
    else if(CHANGE_USB_A0B1 == command)
    {
        ba.append(15);
        ba.append("Change Usb A0B1");
        ba.append(len);
    }
    else if(CHANGE_USB_A1B0 == command)
    {
        ba.append(15);
        ba.append("Change Usb A1B0");
        ba.append(len);
    }
    else if(CHANGE_USB_A1B1 == command)
    {
        ba.append(15);
        ba.append("Change Usb A1B1");
        ba.append(len);
    }
    else if(GET_VOLTAGE == command)
    {
        ba.append(11);
        ba.append("Get Voltage");
        ba.append(len);
    }
    else if(GET_DEVICE == command)
    {
        ba.append(10);
        ba.append("Get Device");
        ba.append(len);
    }
    else if(GET_FIXTURE == command)
    {
        ba.append(11);
        ba.append("Get Fixture");
        ba.append(len);
    }
    for(i= 0; i < ba.length(); ++i)
    {
        sum += ba.at(i);
    }

    qDebug()<<sum;
    check1 = sum/256;
    check2 = sum%256;
    ba.append(check1);
    ba.append(check2);
    qDebug()<<"@@@@@@@@@@@@@@@@@"<<ba.toHex();
    this->write(ba);
}

