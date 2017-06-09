#include "deviceSerial.h"

DeviceSerial *DeviceSerial::instance = 0;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DeviceSerial *DeviceSerial::getInstance()
{
    if(0 == instance)
    {
        instance = new DeviceSerial();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  DeviceSerial
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DeviceSerial::DeviceSerial()
    : QThread()
{
    qRegisterMetaType<CommandType>("CommandType");
    qRegisterMetaType<QList<QString>>("QList<QString>");
    slot_refresh_port();
    data_init();
    connect_init();
    moveToThread(this);
    this->start();

//    slot_open_port("COM5");
//    slot_write_data(BURN_FIRMWARE_START);
//    slot_write_data(BURN_FIRMWARE_END);
//    slot_write_data(CHANGE_USB_A0B0);
//    slot_write_data(CHANGE_USB_A0B1);
//    slot_write_data(CHANGE_USB_A1B0);
//    slot_write_data(CHANGE_USB_A1B1);

    timerRefresh->start();
}


/*******************************************************************************
* Function Name  :  get_listPort
* Description    :  获取串口列表
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QList<QString> DeviceSerial::get_listPort()
{
    return listPort;
}

/*******************************************************************************
* Function Name  :  slot_recive_data
* Description    :  接收数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DeviceSerial::slot_recive_data()
{

}

/*******************************************************************************
* Function Name  :  slot_read_data
* Description    :  接收到数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DeviceSerial::slot_read_data()
{

}




/*******************************************************************************
* Function Name  :  slot_close_port
* Description    :  关闭串口
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DeviceSerial::slot_close_port()
{
    serialPort->close();
    emit signal_closePort_result(0);
}



