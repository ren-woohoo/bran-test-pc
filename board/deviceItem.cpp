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
* Function Name  :  adb_shell
* Description    :  执行命令
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString DeviceItem::adb_shell(QString strCmd)
{
    QString cmd = QString("bin\\adb\\adb -s %1 shell %2").arg(device).arg(strCmd);
    QProcess *process = new QProcess;
    process->start("cmd", QStringList()<<"/c"<<cmd);
    process->waitForStarted(2000);
    process->waitForFinished(5000);
    QString result = QString::fromLocal8Bit(process->readAllStandardOutput());
    result = result.trimmed();
    if(result.isEmpty())
    {
        result = QString::fromLocal8Bit(process->readAllStandardError());
        result = result.trimmed();
    }
    if(process)
    {
        process->close();
        delete process;
        process = 0;
    }
    return result;
}

/*******************************************************************************
* Function Name  :  adb_push
* Description    :  推送文件
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DeviceItem::adb_push(QString strCmd)
{
    QString cmd = QString("bin\\adb\\adb -s %1 push %2").arg(device).arg(strCmd);
    QProcess *process = new QProcess;
    process->start("cmd", QStringList()<<"/c"<<cmd);
    process->waitForStarted(2000);
    process->waitForFinished(5000);
    if(process)
    {
        process->close();
        delete process;
        process = 0;
    }
}

/*******************************************************************************
* Function Name  :  excute_cmd
* Description    :  执行命令
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString DeviceItem::excute_cmd(QString strCmd)
{
    QString cmd = QString("bin\\adb\\adb -s %1 shell %2").arg(device).arg(strCmd);
    QByteArray ba = cmd.toLatin1();
    char *cCmd = ba.data();
    FILE  *stream;
    char  buf[4096];;
    QString str;
    stream = _popen(cCmd, "r");
    fread(buf, sizeof(char), sizeof(buf),  stream);
    _pclose(stream);
    str = buf;
    str = str.trimmed();
    return str;
}
