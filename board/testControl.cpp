#include "testControl.h"

/*******************************************************************************
* Function Name  :  TestControl
* Description    :  测试项
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
TestControl::TestControl(QWidget *parent)
    : QThread()
{
    qRegisterMetaType<InfoMIIO>("InfoMIIO");
    qRegisterMetaType<InfoTest>("InfoTest");

    widgetItem = new WidgetTestItem(parent);
    data_init();
    connect_init();
    moveToThread(this);
}

/*******************************************************************************
* Function Name  :  data_init
* Description    :  数据初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::data_init()
{
    progress = 0;
    isFailed = false;

    serverUser = ServerUser::getInstance();

    serialItem = new SerialItem();
    deviceItem = new DeviceItem();
    serverSync = new ServerSync();
    serverMIIO = new ServerMIIO();

    deviceItem->moveToThread(this);
    serialItem->moveToThread(this);
    serverSync->moveToThread(this);
    serverMIIO->moveToThread(this);

    timerRetry = new QTimer(this);
    timerRetry->setInterval(1000);

    testCPU = new TestCPU(deviceItem, serialItem);
    testRTC = new TestRTC(deviceItem, serialItem);
    testGravity = new TestGravity(deviceItem, serialItem);
    testWiFi = new TestWiFi(deviceItem, serialItem);
    testUSB = new TestUSB(deviceItem, serialItem);
    testVOL = new TestVOL(deviceItem, serialItem);
    testMIIO = new TestMIIO(deviceItem, serialItem);

    testCPU->moveToThread(this);
    testRTC->moveToThread(this);
    testGravity->moveToThread(this);
    testWiFi->moveToThread(this);
    testUSB->moveToThread(this);
    testVOL->moveToThread(this);
    testMIIO->moveToThread(this);
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::connect_init()
{
    connect(serverUser, SIGNAL(signal_update_user(QString,QString)), serverMIIO, SLOT(slot_update_user(QString,QString)));
    connect(serverUser, SIGNAL(signal_update_user(QString,QString)), serverSync, SLOT(slot_update_user(QString,QString)));

    connect(serialItem, SIGNAL(signal_openPort_result(int)), this ,SLOT(slot_openPort_result(int)));
    connect(serialItem, SIGNAL(signal_getDevice_feedback(QString)), this, SLOT(slot_getDevice_feedback(QString)));
    connect(serialItem, SIGNAL(signal_getFixture_feedback(QString)), this, SLOT(slot_getFixture_feedback(QString)));

    connect(testCPU, SIGNAL(signal_test_result(int,QString)), this, SLOT(slot_testCPU_result(int,QString)));
    connect(testRTC, SIGNAL(signal_test_result(int,QString)), this, SLOT(slot_testRTC_result(int,QString)));
    connect(testGravity, SIGNAL(signal_test_result(int,QString)), this, SLOT(slot_testGravity_result(int,QString)));
    connect(testWiFi, SIGNAL(signal_test_result(int,QString)), this, SLOT(slot_testWiFi_result(int,QString)));
    connect(testUSB, SIGNAL(signal_test_result(int,QString)), this, SLOT(slot_testUSB_result(int,QString)));
    connect(testVOL, SIGNAL(signal_test_result(int,QString)), this, SLOT(slot_testVOL_result(int,QString)));
    connect(testMIIO, SIGNAL(signal_test_result(int,QString)), this, SLOT(slot_testMIIO_result(int,QString)));
    connect(testMIIO, SIGNAL(signal_update_infoMIIO(InfoMIIO)), this, SLOT(slot_update_infoMIIO(InfoMIIO)));

    connect(this, SIGNAL(signal_sync_testPassed(InfoTest)), serverSync, SLOT(slot_sync_testPassed(InfoTest)));
    connect(this, SIGNAL(signal_sync_testFailed(InfoTest)), serverSync, SLOT(slot_sync_testFailed(InfoTest)));
    connect(serverSync, SIGNAL(signal_syncTest_success()), this, SLOT(slot_syncTest_success()));
    connect(serverSync, SIGNAL(signal_syncTest_failed(QString)), this, SLOT(slot_syncTest_failed(QString)));


    connect(testMIIO, SIGNAL(signal_get_infoMIIO(QString)), serverMIIO, SLOT(slot_fetch_mac(QString)));
    connect(serverMIIO, SIGNAL(signal_fetch_success(InfoMIIO)), testMIIO, SLOT(slot_getInfoMIIO_success(InfoMIIO)));
    connect(serverMIIO, SIGNAL(signal_fetch_failed(QString)), testMIIO, SLOT(slot_getInfoMIIO_failed(QString)));
}

/*******************************************************************************
* Function Name  :  slot_add_fixture
* Description    :  添加治具
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_add_fixture(QString port)
{
    infoFixture.clear();
    infoFixture.port = port;
    serialItem->set_port(port);
    if(serialItem->open_port(port))
    {
        // 获取治具唯一id
        serialItem->write_data(GET_FIXTURE);
        timerRetry->start();
    }
    else
    {
        // 发送连接治具失败信号
        emit signal_connectFixture_result(-1);
    }
}

/*******************************************************************************
* Function Name  :  slot_delete_fixture
* Description    :  移除治具
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_delete_fixture()
{
    serialItem->close_port();
    infoFixture.clear();
}

/*******************************************************************************
* Function Name  :  slot_getFixture_feedback
* Description    :  获取治具信息反馈
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_getFixture_feedback(QString replyData, QString data)
{
    if(!data.contains("Fail"))
    {
        // 解析出治具的sn
        infoFixture.id = data;
        emit signal_update_fixtureSN(infoFixture.id);
    }
}

/*******************************************************************************
* Function Name  :  slot_get_device
* Description    :  采集设备SN
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_get_device()
{
    if(!infoFixture.isEmpty() && !infoFixture.isInvalid() && !infoFixture.isReady())
    {
        // 获取此时的设备号
        serialItem->write_data(GET_DEVICE);
    }
}

/*******************************************************************************
* Function Name  :  slot_getDevice_feedback
* Description    :  获取设备信息反馈
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_getDevice_feedback(QString replyData, QString data)
{
    if(!data.contains("Fail"))
    {
        // 解析出设备的sn
        infoFixture.deviceSN = data;
        emit signal_update_deviceSN(infoFixture.deviceSN);
        testCPU->set_deviceSN(infoFixture.deviceSN);
        testMIIO->set_deviceSN(infoFixture.deviceSN);
    }
}

/*******************************************************************************
* Function Name  :  slot_set_device
* Description    :  设置设备号
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_set_deviceADB(QString device)
{
    this->deviceADB = device;
    deviceItem->set_device(device);
    testCPU->set_deviceADB(device);
}

/*******************************************************************************
* Function Name  :  slot_start_test
* Description    :  开始测试
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_start_test()
{
    qDebug()<<"CPU START TEST";
    progress = 0;
    isFailed = false;
    infoTest.clear();
    testCPU->start_test();
}

/*******************************************************************************
* Function Name  :  slot_testCPU_result
* Description    :  检测CPU
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_testCPU_result(int result,QString debugInfo)
{
    emit signal_testCPU_result(result,debugInfo);
    progress += 10;
    emit signal_update_progress(progress);
    qDebug()<<"RTC START TEST";
    testRTC->start_test();
    if(result != 0)
    {
        isFailed = true;
        infoTest.infoResult.cpu = false;
    }
    else
    {
        infoTest.infoResult.cpu = true;
    }
}

/*******************************************************************************
* Function Name  :  slot_testRTC_result
* Description    :  检测RTC
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_testRTC_result(int result,QString debugInfo)
{
    emit signal_testRTC_result(result,debugInfo);
    progress += 10;
    emit signal_update_progress(progress);
    qDebug()<<"G-SENSOR START TEST";
    testGravity->start_test();
    if(result != 0)
    {
        isFailed = true;
        infoTest.infoResult.rtc = false;
    }
    else
    {
        infoTest.infoResult.rtc = true;
    }
}

/*******************************************************************************
* Function Name  :  slot_testGravity_result
* Description    :  测试G-Sensor
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_testGravity_result(int result,QString debugInfo)
{
    emit signal_testGravity_result(result,debugInfo);
    progress += 10;
    emit signal_update_progress(progress);
    qDebug()<<"WIFI START TEST";
    testWiFi->start_test();
    if(result != 0)
    {
        isFailed = true;
        infoTest.infoResult.gravity = false;
    }
    else
    {
        infoTest.infoResult.gravity = true;
    }
}

/*******************************************************************************
* Function Name  :  slot_testWiFi_result
* Description    :  测试WiFi结果
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_testWiFi_result(int result,QString debugInfo)
{
    emit signal_testWiFi_result(result,debugInfo);
    progress += 10;
    emit signal_update_progress(progress);
    qDebug()<<"USB_VBUS START TEST";
    testUSB->start_test();
    if(result != 0)
    {
        isFailed = true;
        infoTest.infoResult.wifi = false;
    }
    else
    {
        infoTest.infoResult.wifi = true;
    }
}

/*******************************************************************************
* Function Name  :  slot_testUSB_result
* Description    :  测试USB结果
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_testUSB_result(int result,QString debugInfo)
{
    emit signal_testUSB_result(result,debugInfo);
    progress += 15;
    emit signal_update_progress(progress);
    qDebug()<<"VOL START TEST";
    testVOL->start_test();
    if(result != 0)
    {
        isFailed = true;
        infoTest.infoResult.usb = false;
    }
    else
    {
        infoTest.infoResult.usb = true;
    }
}

/*******************************************************************************
* Function Name  :  slot_testVOL_result
* Description    :  测试电压值结果
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_testVOL_result(int result,QString debugInfo)
{
    emit signal_testVOL_result(result,debugInfo);
    progress += 15;
    emit signal_update_progress(progress);
    if(result != 0)
    {
        isFailed = true;
        infoTest.infoResult.vol = false;
    }
    else
    {
        infoTest.infoResult.vol = true;
    }
    if(isFailed)
    {
        // 直接上报服务器
        emit signal_sync_testFailed(infoTest);
    }
    else
    {
        testMIIO->start_test();
    }
}

/*******************************************************************************
* Function Name  :  slot_testMIIO_result
* Description    :  写入米家did结果
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_testMIIO_result(int result,QString debugInfo)
{
    qDebug()<<"###############################################################";
    emit signal_testMIIO_result(result,debugInfo);
    progress += 20;
    emit signal_update_progress(progress);
    if(result != 0)
    {
        isFailed = true;
        infoTest.infoResult.miio = false;
        emit signal_sync_testFailed(infoTest);
    }
    else
    {
        emit signal_sync_testPassed(infoTest);
        infoTest.infoResult.miio = true;
    }

}

/*******************************************************************************
* Function Name  :  slot_update_infoMIIO
* Description    :  刷新米家信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_update_infoMIIO(InfoMIIO infoMIIO)
{
    qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";
    infoTest.infoMIIO = infoMIIO;
    qDebug()<<"***********************************"<<infoTest.infoMIIO.did;
}

/*******************************************************************************
* Function Name  :  slot_syncTest_result
* Description    :  同步测试结果
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_syncTest_success()
{
    emit signal_syncTest_result(0,debugInfoSync);
    progress += 10;
    emit signal_update_progress(progress);

    // 有错误发生
    if(isFailed)
    {
        emit signal_test_result(-1);
    }
    else
    {
        emit signal_test_result(0);
    }
}

/*******************************************************************************
* Function Name  :  slot_syncTest_failed
* Description    :  同步测试错误
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_syncTest_failed(QString)
{
    emit signal_syncTest_result(-1,debugInfoSync);
    emit signal_test_result(-1);
}

/*******************************************************************************
* Function Name  :  slot_stop_test
* Description    :  停止检测
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_stop_test()
{

}

