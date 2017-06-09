#include "fileConfig.h"

FileConfig *FileConfig::instance = 0;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
FileConfig *FileConfig::getInstance()
{
    if(0 == instance)
    {
        instance = new FileConfig();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  FilelConfig
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
FileConfig::FileConfig(QObject *parent) : QObject(parent)
{
    list_init();
    config_init();
    config_save();
}


/*******************************************************************************
* Function Name  :  config_init
* Description    :  配置初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FileConfig::config_init()
{
    QFile file(SYSTEM_CONFIG_PATH);
    if(file.open(QIODevice::ReadOnly))
    {
        //设置字符编码
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
        //声明并初始化数据流对象
        QTextStream stream(&file);
        QString strReadLine;
        //读取文件内容
        while ( !stream.atEnd())
        {
            strReadLine = stream.readLine();
            QStringList list = strReadLine.split("=");
            if(list.size() == 2)
            {
                QMutableListIterator<SystemValue> i(globalList);
                while(i.hasNext())
                {
                    if(i.next().name == strReadLine.split("=").at(0).trimmed())
                    {
                        SystemValue g(strReadLine.split("=").at(0).trimmed(),strReadLine.split("=").at(1).trimmed());
                        i.setValue(g);
                    }
                }
            }
        }
        //关闭文件
        file.close();
    }
    else
    {
        qDebug()<<"NO CONFIG FILE";
    }
}


/*******************************************************************************
* Function Name  :  list_init
* Description    :  初始化全局配置
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FileConfig::list_init(void)
{
    // 初始化全局配置列表
    SystemValue sysConfig0(DATA_ERROR,      DEFAULT_DATA_ERROR);
    SystemValue sysConfig1(REF_LED,         DEFAULT_REF_LED);
    SystemValue sysConfig2(REF_TVOC,        DEFAULT_REF_TVOC);
    SystemValue sysConfig3(REF_AVCC,        DEFAULT_REF_AVCC);
    SystemValue sysConfig4(REF_WIFI,        DEFAULT_REF_WIFI);
    SystemValue sysConfig5(REF_RTC,         DEFAULT_REF_RTC);
    SystemValue sysConfig6(REF_IPSOUT,      DEFAULT_REF_IPSOUT);
    SystemValue sysConfig7(REF_DRAM,        DEFAULT_REF_DRAM);
    SystemValue sysConfig8(REF_5V,          DEFAULT_REF_5V);

    globalList.append(sysConfig0);
    globalList.append(sysConfig1);
    globalList.append(sysConfig2);
    globalList.append(sysConfig3);
    globalList.append(sysConfig4);
    globalList.append(sysConfig5);
    globalList.append(sysConfig6);
    globalList.append(sysConfig7);
    globalList.append(sysConfig8);

}// End of void SysConfig::list_init(void)

/*******************************************************************************
* Function Name  :  config_save
* Description    :  保存设置
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FileConfig::config_save()
{
    QFile file(SYSTEM_CONFIG_PATH);
    if(file.exists())
    {
        //        qDebug("exist");
    }
    if(file.open(QFile::WriteOnly))
    {
        //声明并初始化数据流对象
        QTextStream stream(&file);
        stream.setCodec("utf-8");
        foreach(SystemValue object,globalList)
        {
            QString str = object.name + " = " + object.value;
            stream<<str<<"\n";
        }
        file.close();
    }
}

/*******************************************************************************
* Function Name  :  get_value
* Description    :  获取指定值
* Input          :  QString name    指定值
* Output         :  None
* Return         :  None
*******************************************************************************/
QString FileConfig::get_value(QString name)
{
    if(globalList.isEmpty())
    {
        config_init();
    }
    foreach(SystemValue object,globalList)
    {
        if(name == object.name)
        {
            return object.value;
        }
    }
    return NULL;
}

/*******************************************************************************
* Function Name  :  set_value
* Description    :  设置固定值
* Input          :  QString name    字段名
*                   QString value   值
* Output         :  None
* Return         :  None
*******************************************************************************/
void FileConfig::set_value(QString name, QString value)
{
    if(globalList.isEmpty())
    {
        config_init();
    }
    QMutableListIterator<SystemValue> i(globalList);

    while(i.hasNext())
    {
        if(i.next().name == name)
        {
            SystemValue g(name,value);
            i.setValue(g);
        }
    }
    config_save();
}
