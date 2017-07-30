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
    timer->setInterval(3000);

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
    connect(serialItem, SIGNAL(signal_getVoltage_feedback(QString,QString)), this, SLOT(slot_getVoltage_feedback(QString,QString)));
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_serial_timeout()));
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
    isFailed = false;
    debugInfo = "START TEST VOLTAGE ...\n";
    cmd = serialItem->package_cmd(GET_VOLTAGE);
    emit signal_write_data(cmd);
    timer->start();
}

/*******************************************************************************
* Function Name  :  slot_getVoltage_feedback
* Description    :  串口返回
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestVOL::slot_getVoltage_feedback(QString replyData,QString data)
{
    debugInfo.append(QString("RECIVE:%1, DATA:%2, ASCII:%3\n").arg(replyData).arg(data).arg(QString(QByteArray::fromHex(data.toLatin1()))));
    if(serialItem->package_cmd(GET_VOLTAGE) != cmd)
    {
        return;
    }
    data = QString(QByteArray::fromHex(data.toLatin1()));
    timer->stop();
    if(data.contains("=") && data.contains(","))
    {
        // 开始分析电压值
        infoVol.clear();
        QStringList listData = data.split(",");
        QStringList list;
        for(int i = 0; i< listData.length(); ++i)
        {
            if(QString(listData.at(i)).contains("Vol0"))
            {
                list = (listData.at(i)).split("=");
                if(list.length() == 2)
                {
                    infoVol.vol0_LED = QString(list.at(1)).trimmed().toFloat();
                }
            }
            else if(QString(listData.at(i)).contains("Vol1"))
            {
                list = (listData.at(i)).split("=");
                if(list.length() == 2)
                {
                    infoVol.vol1_TVOC = QString(list.at(1)).trimmed().toFloat();
                }
            }
            else if(QString(listData.at(i)).contains("Vol2"))
            {
                list = (listData.at(i)).split("=");
                if(list.length() == 2)
                {
                    infoVol.vol2_AVCC = QString(list.at(1)).trimmed().toFloat();
                }
            }
            else if(QString(listData.at(i)).contains("Vol3"))
            {
                list = (listData.at(i)).split("=");
                if(list.length() == 2)
                {
                    infoVol.vol3_WIFI = QString(list.at(1)).trimmed().toFloat();
                }
            }
            else if(QString(listData.at(i)).contains("Vol4"))
            {
                list = (listData.at(i)).split("=");
                if(list.length() == 2)
                {
                    infoVol.vol4_RTC = QString(list.at(1)).trimmed().toFloat();
                }
            }
            else if(QString(listData.at(i)).contains("Vol5"))
            {
                list = (listData.at(i)).split("=");
                if(list.length() == 2)
                {
                    infoVol.vol5_IPSOUT = QString(list.at(1)).trimmed().toFloat();
                }
            }
            else if(QString(listData.at(i)).contains("Vol6"))
            {
                list = (listData.at(i)).split("=");
                if(list.length() == 2)
                {
                    infoVol.vol6_DRAM = QString(list.at(1)).trimmed().toFloat();
                }
            }
            else if(QString(listData.at(i)).contains("Vol7"))
            {
                list = (listData.at(i)).split("=");
                if(list.length() == 2)
                {
                    infoVol.vol7_5V = QString(list.at(1)).trimmed().toFloat();
                }
            }

        }
        emit signal_update_infoVOL(infoVol);
        if(analyse_data(infoVol))
        {
            debugInfo.append("END TEST VOLTAGE --- PASS!!!");
            emit signal_test_result(0, debugInfo);
        }
        else
        {
            debugInfo.append("END TEST VOLTAGE --- FAIL!!!");
            emit signal_test_result(-1, debugInfo);
        }
    }
    else
    {
        debugInfo.append("END TEST VOLTAGE --- FAIL!!!");
        emit signal_test_result(-1, debugInfo);
    }
}


/*******************************************************************************
* Function Name  :  slot_testData_success
* Description    :  分析设备数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool TestVOL::analyse_data(InfoVol infoVol)
{
    if(is_valid(infoVol.vol0_LED, refDatas.dataVol0))
    {
        debugInfo.append(QString("VOL_LED=%1,Normal.\n").arg(infoVol.vol0_LED));
    }
    else
    {
        isFailed = true;
        debugInfo.append(QString("VOL_LED=%1,Error!\n").arg(infoVol.vol0_LED));
    }
    if(is_valid(infoVol.vol1_TVOC, refDatas.dataVol1))
    {
        debugInfo.append(QString("VOL_TVOC=%1,Normal.\n").arg(infoVol.vol1_TVOC));
    }
    else
    {
        debugInfo.append(QString("VOL_TVOC=%1,Error!\n").arg(infoVol.vol1_TVOC));
        isFailed = true;
    }
    if(is_valid(infoVol.vol2_AVCC, refDatas.dataVol2))
    {
        debugInfo.append(QString("VOL_AVCC=%1,Normal.\n").arg(infoVol.vol2_AVCC));
    }
    else
    {
        debugInfo.append(QString("VOL_AVCC=%1,Error!\n").arg(infoVol.vol2_AVCC));
        isFailed = true;
    }
    if(is_valid(infoVol.vol3_WIFI, refDatas.dataVol3))
    {
        debugInfo.append(QString("VOL_WIFI=%1,Normal.\n").arg(infoVol.vol3_WIFI));
    }
    else
    {
        debugInfo.append(QString("VOL_WIFI=%1,Error!\n").arg(infoVol.vol3_WIFI));
        isFailed = true;
    }
    if(is_valid(infoVol.vol4_RTC, refDatas.dataVol4))
    {
        debugInfo.append(QString("VOL_RTC=%1,Normal.\n").arg(infoVol.vol4_RTC));
    }
    else
    {
        debugInfo.append(QString("VOL_RTC=%1,Error!\n").arg(infoVol.vol4_RTC));
        isFailed = true;
    }
    if(is_valid(infoVol.vol5_IPSOUT, refDatas.dataVol5))
    {
        debugInfo.append(QString("VOL_IPSOUT=%1,Normal.\n").arg(infoVol.vol5_IPSOUT));
    }
    else
    {
        debugInfo.append(QString("VOL_IPSOUT=%1,Error!\n").arg(infoVol.vol5_IPSOUT));
        isFailed = true;
    }
    if(is_valid(infoVol.vol6_DRAM, refDatas.dataVol6))
    {
        debugInfo.append(QString("VOL_DRAM=%1,Normal.\n").arg(infoVol.vol6_DRAM));
    }
    else
    {
        debugInfo.append(QString("VOL_DRAM=%1,Error!\n").arg(infoVol.vol6_DRAM));
        isFailed = true;
    }
    if(is_valid(infoVol.vol7_5V, refDatas.dataVol7))
    {
        debugInfo.append(QString("VOL_5V=%1,Normal.\n").arg(infoVol.vol7_5V));
    }
    else
    {
        debugInfo.append(QString("VOL_5V=%1,Error!\n").arg(infoVol.vol7_5V));
        isFailed = true;
    }
    if(isFailed)
    {
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
void TestVOL::slot_serial_timeout()
{
    debugInfo.append("SERIAL TIME OUT!!!\n");
    debugInfo.append("END TEST VOLTAGE --- FAIL!!!");
    emit signal_test_result(-1, debugInfo);
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
