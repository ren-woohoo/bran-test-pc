#include "deviceItem.h"

/*******************************************************************************
* Function Name  :  DeviceItem
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DeviceItem::DeviceItem(QObject *parent) : QObject(parent)
{

}

/*******************************************************************************
* Function Name  :  set_device
* Description    :  设置设备
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DeviceItem::set_device(QString device)
{
    this->device = device;
}

/*******************************************************************************
* Function Name  :  slot_excute_cmd
* Description    :  执行命令
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString DeviceItem::excute_cmd(QString strCmd)
{
    QProcess *process = new QProcess;
//    QStringList args;

    QString cmd = QString("adb -s %1 %2").arg(device).arg(strCmd);
    process->start("cmd", QStringList()<<"/c"<<cmd);
    process->waitForStarted();
    process->waitForFinished();
    QString result = QString::fromLocal8Bit(process->readAllStandardOutput());
    if(process)
    {
        process->close();
        delete process;
        process = 0;
    }
    return result;
}
