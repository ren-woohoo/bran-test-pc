#include "sysUser.h"

SysUser *SysUser::instance = 0;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysUser *SysUser::getInstance()
{
    if(instance == 0)
    {
        instance = new SysUser();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  SysUser
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysUser::SysUser()
    :QThread()
{
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
void SysUser::data_init()
{
    serverUser = ServerUser::getInstance();
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysUser::connect_init()
{
    connect(this, SIGNAL(signal_user_login(QString,QString)), serverUser, SLOT(slot_user_login(QString,QString)));
    connect(this, SIGNAL(signal_user_modify(QString)), serverUser, SLOT(slot_user_modify(QString)));
    connect(serverUser, SIGNAL(signal_login_result(int)), this, SIGNAL(signal_login_result(int)));
    connect(serverUser, SIGNAL(signal_modify_result(int)), this, SIGNAL(signal_modify_result(int)));
}
