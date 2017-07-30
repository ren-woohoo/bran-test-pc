#include "baseTestItem.h"

BaseTestItem::BaseTestItem(DeviceItem* deviceItem, SerialItem *serialItem) : QObject()
{
    this->deviceItem = deviceItem;
    this->serialItem = serialItem;
}

/*******************************************************************************
* Function Name  :  start_test
* Description    :  开始测试
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void BaseTestItem::start_test()
{

}

/*******************************************************************************
* Function Name  :  get_debugInfo
* Description    :  获取调试信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString BaseTestItem::get_debugInfo()
{
    return debugInfo;
}
