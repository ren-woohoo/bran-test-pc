#include "testVOL.h"

/*******************************************************************************
* Function Name  :  TestVOL
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
TestVOL::TestVOL(DeviceItem *deviceItem, SerialItem *serialItem)
    : BaseTestItem(deviceItem, serialItem)
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
void TestVOL::data_init()
{
    sysData = SysData::getInstance();
    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(2000);
    refDatas.init();
    refDatas = sysData->get_refDatas();
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestVOL::connect_init()
{
    connect(serialItem, SIGNAL(signal_getVoltage_feedback(QString)), this, SLOT(slot_getVoltage_feedback(QString)));
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_getVoltage_timeout()));
    connect(sysData, SIGNAL(signal_update_refDatas(RefDatas)), this, SLOT(slot_change_refDatas(RefDatas)));
}

/*******************************************************************************
* Function Name  :  start_test
* Description    :  开始测试
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestVOL::start_test()
{
    debugInfo = "START TEST VOLTAGE ...\n";
    numFailed = 0;
    serialItem->write_data(GET_VOLTAGE);
    timer->start();
}

/*******************************************************************************
* Function Name  :  slot_getVoltage_feedback
* Description    :  串口返回
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestVOL::slot_getVoltage_feedback(QString replyData)
{
    debugInfo.append(QString("%1\n").arg(replyData));

    timer->stop();
    if(replyData.contains("=") && replyData.contains(","))
    {
        replyData = replyData.mid(0,(replyData.length()-2));

        // 开始分析电压值
        branData.clear();
        QStringList listData = replyData.split(",");
        QStringList list;
        for(int i = 0; i< listData.length(); ++i)
        {
            if(QString(listData.at(i)).contains("Vol0"))
            {
                list = (listData.at(i)).split("=");
                if(list.length() == 2)
                {
                    branData.vol0_LED = QString(list.at(1)).trimmed().toFloat();
                }
            }
            else if(QString(listData.at(i)).contains("Vol1"))
            {
                list = (listData.at(i)).split("=");
                if(list.length() == 2)
                {
                    branData.vol1_TVOC = QString(list.at(1)).trimmed().toFloat();
                }
            }
            else if(QString(listData.at(i)).contains("Vol2"))
            {
                list = (listData.at(i)).split("=");
                if(list.length() == 2)
                {
                    branData.vol2_AVCC = QString(list.at(1)).trimmed().toFloat();
                }
            }
            else if(QString(listData.at(i)).contains("Vol3"))
            {
                list = (listData.at(i)).split("=");
                if(list.length() == 2)
                {
                    branData.vol3_WIFI = QString(list.at(1)).trimmed().toFloat();
                }
            }
            else if(QString(listData.at(i)).contains("Vol4"))
            {
                list = (listData.at(i)).split("=");
                if(list.length() == 2)
                {
                    branData.vol4_RTC = QString(list.at(1)).trimmed().toFloat();
                }
            }
            else if(QString(listData.at(i)).contains("Vol5"))
            {
                list = (listData.at(i)).split("=");
                if(list.length() == 2)
                {
                    branData.vol5_IPSOUT = QString(list.at(1)).trimmed().toFloat();
                }
            }
            else if(QString(listData.at(i)).contains("Vol6"))
            {
                list = (listData.at(i)).split("=");
                if(list.length() == 2)
                {
                    branData.vol6_DRAM = QString(list.at(1)).trimmed().toFloat();
                }
            }
            else if(QString(listData.at(i)).contains("Vol7"))
            {
                list = (listData.at(i)).split("=");
                if(list.length() == 2)
                {
                    branData.vol7_5V = QString(list.at(1)).trimmed().toFloat();
                }
            }

        }
        if(analyse_data(branData))
        {
            // 测试成功
            emit signal_test_result(0, debugInfo);
            debugInfo.append("END TEST VOLTAGE ------ SUCCESS!!!");
        }
        else
        {
            emit signal_test_result(-1, debugInfo);
            debugInfo.append("END TEST VOLTAGE ------ FAILED!!!");
        }
    }
    else
    {
        if( ++numFailed < 3)
        {
            // 重试一次
            emit serialItem->write_data(GET_VOLTAGE);
            timer->start();
        }
        else
        {
            // 发送失败
            emit signal_test_result(-1, debugInfo);
            debugInfo.append("get voltage failed!!!\n");
            debugInfo.append("END TEST VOLTAGE ------ FAILED!!!");
            numFailed = 0;
        }
    }
}


/*******************************************************************************
* Function Name  :  slot_testData_success
* Description    :  分析设备数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool TestVOL::analyse_data(InfoVol branData)
{
    if(is_valid(branData.vol0_LED, refDatas.dataVol0))
    {
        debugInfo.append(QString("VOL_LED=%1,Normal.\n").arg(branData.vol0_LED));
    }
    else
    {
        debugInfo.append(QString("VOL_LED=%1,Error!\n").arg(branData.vol0_LED));
        return false;
    }
    if(is_valid(branData.vol1_TVOC, refDatas.dataVol1))
    {
        debugInfo.append(QString("VOL_TVOC=%1,Normal.\n").arg(branData.vol1_TVOC));
    }
    else
    {
        debugInfo.append(QString("VOL_TVOC=%1,Error!\n").arg(branData.vol1_TVOC));
        return false;
    }
    if(is_valid(branData.vol2_AVCC, refDatas.dataVol2))
    {
        debugInfo.append(QString("VOL_AVCC=%1,Normal.\n").arg(branData.vol2_AVCC));
    }
    else
    {
        debugInfo.append(QString("VOL_AVCC=%1,Error!\n").arg(branData.vol2_AVCC));
        return false;
    }
    if(is_valid(branData.vol3_WIFI, refDatas.dataVol3))
    {
        debugInfo.append(QString("VOL_WIFI=%1,Normal.\n").arg(branData.vol3_WIFI));
    }
    else
    {
        debugInfo.append(QString("VOL_WIFI=%1,Error!\n").arg(branData.vol3_WIFI));
        return false;
    }
    if(is_valid(branData.vol4_RTC, refDatas.dataVol4))
    {
        debugInfo.append(QString("VOL_RTC=%1,Normal.\n").arg(branData.vol4_RTC));
    }
    else
    {
        debugInfo.append(QString("VOL_RTC=%1,Error!\n").arg(branData.vol4_RTC));
        return false;
    }
    if(is_valid(branData.vol5_IPSOUT, refDatas.dataVol5))
    {
        debugInfo.append(QString("VOL_IPSOUT=%1,Normal.\n").arg(branData.vol5_IPSOUT));
    }
    else
    {
        debugInfo.append(QString("VOL_IPSOUT=%1,Error!\n").arg(branData.vol5_IPSOUT));
        return false;
    }
    if(is_valid(branData.vol6_DRAM, refDatas.dataVol6))
    {
        debugInfo.append(QString("VOL_DRAM=%1,Normal.\n").arg(branData.vol6_DRAM));
    }
    else
    {
        debugInfo.append(QString("VOL_DRAM=%1,Error!\n").arg(branData.vol6_DRAM));
        return false;
    }
    if(is_valid(branData.vol7_5V, refDatas.dataVol7))
    {
        debugInfo.append(QString("VOL_5V=%1,Normal.\n").arg(branData.vol7_5V));
    }
    else
    {
        debugInfo.append(QString("VOL_5V=%1,Error!\n").arg(branData.vol7_5V));
        return false;
    }
    return true;
}

/*******************************************************************************
* Function Name  :  slot_getVoltage_timeout
* Description    :  获取电压值超时
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestVOL::slot_getVoltage_timeout()
{
    if(++numFailed < 3)
    {
        // 重试一次
        serialItem->write_data(GET_VOLTAGE);
        timer->start();
    }
    else
    {
        emit signal_test_result(-1, debugInfo);
        debugInfo.append("get voltage timeout!!!\n");
        debugInfo.append("GET VOLTAGE ------ FAILED!!!");
        numFailed = 0;
    }
}

/*******************************************************************************
* Function Name  :  slot_change_refDatas
* Description    :  参考数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestVOL::slot_change_refDatas(RefDatas refDatas)
{
    this->refDatas = refDatas;

}

/*******************************************************************************
* Function Name  :  is_valid
* Description    :  是否为有效的
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool TestVOL::is_valid(float value, float refData)
{
    if(value == ERROR_DATA)
    {
        return false;
    }
    if((value < (refData + refDatas.dataError)) && (value >= (refData - refDatas.dataError)))
    {
        return true;
    }
    return false;
}
