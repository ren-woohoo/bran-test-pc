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
    qRegisterMetaType<InfoVol>("InfoVol");
    qRegisterMetaType<InfoTest>("InfoTest");
    qRegisterMetaType<InfoFixture>("InfoFixture");
    widgetItem = new WidgetTestItem(parent);
    widgetTest = new WidgetTest();
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
    testStage = IS_FREE;

    serverUser = ServerUser::getInstance();

    serialItem = new SerialItem();
    deviceItem = new DeviceItem();
    serverSync = new ServerSync();
    serverMIIO = new ServerMIIO();

    deviceItem->moveToThread(this);
    serialItem->moveToThread(this);
    serverSync->moveToThread(this);
    serverMIIO->moveToThread(this);

    timerRetry = new QTimer();
    timerRetry->setInterval(1000);

    testCPU = new TestCPU(deviceItem, serialItem);
    testRTC = new TestRTC(deviceItem, serialItem);
    testGravity = new TestGravity(deviceItem, serialItem);
    testWiFi = new TestWiFi(deviceItem, serialItem);
    testUSB = new TestUSB(deviceItem, serialItem);
    testVOL = new TestVOL(deviceItem, serialItem);
    testMIIO = new TestMIIO(deviceItem, serialItem);
    testSYNC = new TestSYNC(deviceItem, serialItem);

    testCPU->moveToThread(this);
    testRTC->moveToThread(this);
    testGravity->moveToThread(this);
    testWiFi->moveToThread(this);
    testUSB->moveToThread(this);
    testVOL->moveToThread(this);
    testMIIO->moveToThread(this);
    testSYNC->moveToThread(this);
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

    connect(this, SIGNAL(signal_write_data(QString)), serialItem, SLOT(slot_write_data(QString)));
    connect(serialItem, SIGNAL(signal_getDevice_feedback(QString,QString)), this, SLOT(slot_getDevice_feedback(QString,QString)));
    connect(serialItem, SIGNAL(signal_getFixture_feedback(QString,QString)), this, SLOT(slot_getFixture_feedback(QString,QString)));

    connect(testCPU, SIGNAL(signal_test_result(int,QString)), this, SLOT(slot_testCPU_result(int,QString)));
    connect(testRTC, SIGNAL(signal_test_result(int,QString)), this, SLOT(slot_testRTC_result(int,QString)));
    connect(testGravity, SIGNAL(signal_test_result(int,QString)), this, SLOT(slot_testGravity_result(int,QString)));
    connect(testWiFi, SIGNAL(signal_test_result(int,QString)), this, SLOT(slot_testWiFi_result(int,QString)));
    connect(testUSB, SIGNAL(signal_test_result(int,QString)), this, SLOT(slot_testUSB_result(int,QString)));
    connect(testVOL, SIGNAL(signal_test_result(int,QString)), this, SLOT(slot_testVOL_result(int,QString)));
    connect(testMIIO, SIGNAL(signal_test_result(int,QString)), this, SLOT(slot_testMIIO_result(int,QString)));
    connect(testSYNC, SIGNAL(signal_test_result(int,QString)), this, SLOT(slot_testSYNC_result(int,QString)));

    connect(this, SIGNAL(signal_testCPU_result(int,QString)), widgetTest, SLOT(slot_testCPU_result(int,QString)));
    connect(this, SIGNAL(signal_testRTC_result(int,QString)), widgetTest, SLOT(slot_testRTC_result(int,QString)));
    connect(this, SIGNAL(signal_testGravity_result(int,QString)), widgetTest, SLOT(slot_testGravity_result(int,QString)));
    connect(this, SIGNAL(signal_testWiFi_result(int,QString)), widgetTest, SLOT(slot_testWiFi_result(int,QString)));
    connect(this, SIGNAL(signal_testUSB_result(int,QString)), widgetTest, SLOT(slot_testUSB_result(int,QString)));
    connect(this, SIGNAL(signal_testVOL_result(int,QString)), widgetTest, SLOT(slot_testVOL_result(int,QString)));
    connect(this, SIGNAL(signal_testMIIO_result(int,QString)), widgetTest, SLOT(slot_testMIIO_result(int,QString)));
    connect(this, SIGNAL(signal_testSYNC_result(int,QString)), widgetTest, SLOT(slot_testSYNC_result(int,QString)));

    connect(testSYNC, SIGNAL(signal_sync_testPassed(InfoTest)), serverSync, SLOT(slot_sync_testPassed(InfoTest)));
    connect(testSYNC, SIGNAL(signal_sync_testFailed(InfoTest)), serverSync, SLOT(slot_sync_testFailed(InfoTest)));
    connect(serverSync, SIGNAL(signal_syncTest_success(QString)), testSYNC, SLOT(slot_syncTest_success(QString)));
    connect(serverSync, SIGNAL(signal_syncTest_failed(QString)), testSYNC, SLOT(slot_syncTest_failed(QString)));

    connect(testVOL, SIGNAL(signal_update_infoVOL(InfoVol)), this, SLOT(slot_update_infoVOL(InfoVol)));
    connect(testMIIO, SIGNAL(signal_update_infoMIIO(InfoMIIO)), this, SLOT(slot_update_infoMIIO(InfoMIIO)));

    connect(testMIIO, SIGNAL(signal_get_infoMIIO(QString)), serverMIIO, SLOT(slot_fetch_mac(QString)));
    connect(serverMIIO, SIGNAL(signal_fetch_success(InfoMIIO)), testMIIO, SLOT(slot_getInfoMIIO_success(InfoMIIO)));
    connect(serverMIIO, SIGNAL(signal_fetch_failed(QString)), testMIIO, SLOT(slot_getInfoMIIO_failed(QString)));

    connect(this, SIGNAL(signal_test_init()), widgetItem, SLOT(slot_test_init()));
    connect(this, SIGNAL(signal_test_init()), widgetTest, SLOT(slot_test_init()));
    connect(this, SIGNAL(signal_update_infoFixture(InfoFixture)), widgetItem, SLOT(slot_update_infoFixture(InfoFixture)));
    connect(this, SIGNAL(signal_update_progress(int)), widgetItem, SLOT(slot_update_progress(int)));
    connect(this, SIGNAL(signal_test_result(int)), widgetItem, SLOT(slot_test_result(int)));

    connect(widgetItem, SIGNAL(signal_show_widgetTest()), widgetTest, SLOT(show()));
    connect(widgetItem, SIGNAL(signal_show_widgetTest()), widgetTest, SLOT(raise()));
    connect(timerRetry, SIGNAL(timeout()), this, SLOT(slot_retry_samplingFixture()));
}

void TestControl::load_port(QString port)
{
    if(serialItem->open_port(port))
    {
        testStage = IS_PORT_READY;
        infoTest.infoFixture.port = port;
        emit signal_update_infoFixture(infoTest.infoFixture);

        cmd = serialItem->package_cmd(GET_FIXTURE);
        emit signal_write_data(cmd);
        timerRetry->start();
    }
    else
    {
        emit signal_openPort_failed(port);
    }
}

/*******************************************************************************
* Function Name  :  load_fixture
* Description    :  添加治具
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::load_fixture(QString id)
{
    testStage = IS_FIXTURE_READY;
    infoTest.infoFixture.id = id;
    emit signal_update_infoFixture(infoTest.infoFixture);
}

/*******************************************************************************
* Function Name  :  load_deviceSN
* Description    :  添加治具
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::load_deviceSN(QString sn)
{
    testStage = IS_DEVICE_READY;
    infoTest.infoFixture.deviceSN = sn;
    testCPU->set_deviceSN(infoTest.infoFixture.deviceSN);
    testMIIO->set_deviceSN(infoTest.infoFixture.deviceSN);
    emit signal_update_infoFixture(infoTest.infoFixture);
}

/*******************************************************************************
* Function Name  :  load_deviceADB
* Description    :  载入设备ADB
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::load_deviceADB(QString adb)
{
    testStage = IS_MATCHED;
    infoTest.infoFixture.deviceADB = adb;
    deviceItem->set_device(infoTest.infoFixture.deviceADB);
    emit signal_update_infoFixture(infoTest.infoFixture);
}

/*******************************************************************************
* Function Name  :  get_device
* Description    :  查询设备
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::sampling_deviceSN()
{
    if(IS_FIXTURE_READY == testStage)
    {
        cmd = serialItem->package_cmd(GET_DEVICE);
        emit signal_write_data(cmd);
    }
}

/*******************************************************************************
* Function Name  :  slot_delete_fixture
* Description    :  移除治具
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::remove_port()
{
    timerRetry->stop();
    serialItem->close_port();
    testStage = IS_FREE;
    infoTest.clear();
    emit signal_update_infoFixture(infoTest.infoFixture);
}

/*******************************************************************************
* Function Name  :  remove_device
* Description    :  移除设备
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::remove_device()
{
    infoTest.infoFixture.deviceSN.clear();
    infoTest.infoFixture.deviceADB.clear();
    if(infoTest.infoFixture.port.isEmpty())
    {
        testStage = IS_FREE;
    }
    else if(infoTest.infoFixture.id.isEmpty())
    {
        testStage = IS_PORT_READY;
        load_port(infoTest.infoFixture.port);
    }
    else
    {
        testStage = IS_FIXTURE_READY;
    }
}

/*******************************************************************************
* Function Name  :  slot_close_ports
* Description    :  关闭串口
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_close_ports()
{
    serialItem->close_port();
}

/*******************************************************************************
* Function Name  :  slot_retry_samplingFixture
* Description    :  重新检测治具
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_retry_samplingFixture()
{
    if(IS_PORT_READY == testStage)
    {
        cmd = serialItem->package_cmd(GET_FIXTURE);
        emit signal_write_data(cmd);
    }
    else
    {
        timerRetry->stop();
    }
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
    data = QString(QByteArray::fromHex(data.toLatin1()));
    if("Fail" != data)
    {
        timerRetry->stop();
        load_fixture(data);
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
    data = QString(QByteArray::fromHex(data.toLatin1()));
    if("Fail" != data)
    {
        load_deviceSN(data);
    }
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
    if(IS_MATCHED == testStage)
    {
        testStage = IS_TESTING;
        emit signal_test_init();
        progress = 0;
        infoTest.test_init();
        testCPU->start_test();
    }
    else
    {
        emit signal_update_infoFixture(infoTest.infoFixture);
    }
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
    progress += 10;
    emit signal_testCPU_result(result,debugInfo);
    emit signal_update_progress(progress);
    qDebug()<<"RTC START TEST";
    testRTC->start_test();
    if(result != 0)
    {
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
    progress += 10;
    emit signal_testRTC_result(result,debugInfo);
    emit signal_update_progress(progress);
    qDebug()<<"G-SENSOR START TEST";
    testGravity->start_test();
    if(result != 0)
    {
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
    progress += 10;
    emit signal_testGravity_result(result, debugInfo);
    emit signal_update_progress(progress);
    qDebug()<<"WIFI START TEST";
    testWiFi->start_test();
    if(result != 0)
    {
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
    progress += 10;
    emit signal_testWiFi_result(result, debugInfo);
    emit signal_update_progress(progress);
    qDebug()<<"USB_VBUS START TEST";
    testUSB->start_test();
    if(result != 0)
    {
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
    progress += 15;
    emit signal_testUSB_result(result, debugInfo);
    emit signal_update_progress(progress);
    qDebug()<<"VOL START TEST";
    testVOL->start_test();
    if(result != 0)
    {
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
    progress += 15;
    emit signal_testVOL_result(result, debugInfo);
    emit signal_update_progress(progress);
    if(result != 0)
    {
        infoTest.infoResult.vol = false;
    }
    else
    {
        infoTest.infoResult.vol = true;
    }
    if(infoTest.infoResult.vol && infoTest.infoResult.usb && infoTest.infoResult.rtc && infoTest.infoResult.gravity && infoTest.infoResult.cpu)
    {
        testMIIO->start_test();
    }
    else
    {
        testSYNC->start_test(infoTest);
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
    progress += 20;
    emit signal_testMIIO_result(result, debugInfo);
    emit signal_update_progress(progress);
    if(result != 0)
    {
        infoTest.infoResult.miio = false;
    }
    else
    {
        infoTest.infoResult.miio = true;
    }
    testSYNC->start_test(infoTest);
}

/*******************************************************************************
* Function Name  :  slot_testSYNC_result
* Description    :  测试同步结果
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_testSYNC_result(int result, QString debugInfo)
{
    progress += 10;
    emit signal_testSYNC_result(result, debugInfo);
    emit signal_update_progress(progress);
    emit signal_test_end();
    if((0 == result) && infoTest.infoResult.isPassed())
    {
        emit signal_test_result(0);
    }
    else
    {
        emit signal_test_result(-1);
    }
    if(IS_TESTING == testStage)
    {
        testStage = IS_MATCHED;
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
    infoTest.infoMIIO = infoMIIO;
}

/*******************************************************************************
* Function Name  :  slot_update_infoVOL
* Description    :  刷新电压值
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TestControl::slot_update_infoVOL(InfoVol infoVOL)
{
    infoTest.infoVol = infoVOL;
}

QString TestControl::get_device()
{
    return infoTest.infoFixture.deviceADB;
}


QString TestControl::get_deviceSN()
{
    return infoTest.infoFixture.deviceSN;
}

QString TestControl::get_port()
{
    return infoTest.infoFixture.port;
}
