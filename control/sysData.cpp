#include "sysData.h"

SysData *SysData::instance = 0;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysData *SysData::getInstance()
{
    if(instance == 0)
    {
        instance = new SysData();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  SysData
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysData::SysData()
    : QThread()
{
    qRegisterMetaType<RefDatas>("RefDatas");
    data_init();
    connect_init();
    moveToThread(this);
    this->start();
}

/*******************************************************************************
* Function Name  :  data_init
* Description    :  数据初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysData::data_init()
{
    fileConfig = FileConfig::getInstance();
    refDatas.set_error(fileConfig->get_value(DATA_ERROR).toFloat());
    refDatas.dataVol0 = fileConfig->get_value(REF_LED).toFloat();
    refDatas.dataVol1 = fileConfig->get_value(REF_TVOC).toFloat();
    refDatas.dataVol2 = fileConfig->get_value(REF_AVCC).toFloat();
    refDatas.dataVol3 = fileConfig->get_value(REF_WIFI).toFloat();
    refDatas.dataVol4 = fileConfig->get_value(REF_RTC).toFloat();
    refDatas.dataVol5 = fileConfig->get_value(REF_IPSOUT).toFloat();
    refDatas.dataVol6 = fileConfig->get_value(REF_DRAM).toFloat();
    refDatas.dataVol7 = fileConfig->get_value(REF_5V).toFloat();
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysData::connect_init()
{

}

/*******************************************************************************
* Function Name  :  slot_change_refDatas
* Description    :  改变参考值等
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysData::slot_change_refDatas(RefDatas refDatas)
{
    this->refDatas = refDatas;
    emit signal_update_refDatas(refDatas);
    fileConfig->set_value(DATA_ERROR, QString::number(refDatas.dataError));
    fileConfig->set_value(REF_LED, QString::number(refDatas.dataVol0));
    fileConfig->set_value(REF_TVOC, QString::number(refDatas.dataVol1));
    fileConfig->set_value(REF_AVCC, QString::number(refDatas.dataVol2));
    fileConfig->set_value(REF_WIFI, QString::number(refDatas.dataVol3));
    fileConfig->set_value(REF_RTC, QString::number(refDatas.dataVol4));
    fileConfig->set_value(REF_IPSOUT, QString::number(refDatas.dataVol5));
    fileConfig->set_value(REF_DRAM, QString::number(refDatas.dataVol6));
    fileConfig->set_value(REF_5V, QString::number(refDatas.dataVol7));
}

/*******************************************************************************
* Function Name  :  get_refDatas()
* Description    :  获取参考值
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
RefDatas SysData::get_refDatas()
{
    return refDatas;
}
