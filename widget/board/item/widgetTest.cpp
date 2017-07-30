#include "widgetTest.h"
#include "ui_widgetTest.h"

/*******************************************************************************
* Function Name  :  WidgetTest
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WidgetTest::WidgetTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTest)
{
    ui->setupUi(this);
    data_init();
    widget_init();
    connect_init();
}

/*******************************************************************************
* Function Name  :  ~WidgetTest
* Description    :  析构函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WidgetTest::~WidgetTest()
{
    delete ui;
}

/*******************************************************************************
* Function Name  :  data_init
* Description    :  数据初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTest::data_init()
{
    isFailed = false;
}

/*******************************************************************************
* Function Name  :  widget_init
* Description    :  窗口初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTest::widget_init()
{
    widgetDebugInfo = new WidgetDebugInfo();

    statusTest1 = new StatusTest();
    statusTest2 = new StatusTest();
    statusTest3 = new StatusTest();
    statusTest4 = new StatusTest();
    statusTest5 = new StatusTest();
    statusTest6 = new StatusTest();
    statusTest7 = new StatusTest();
    statusTest8 = new StatusTest();


    ui->tableWidget->setCellWidget(0,1,statusTest1);
    ui->tableWidget->setCellWidget(0,4,statusTest2);
    ui->tableWidget->setCellWidget(1,1,statusTest3);
    ui->tableWidget->setCellWidget(1,4,statusTest4);
    ui->tableWidget->setCellWidget(2,1,statusTest5);
    ui->tableWidget->setCellWidget(2,4,statusTest6);
    ui->tableWidget->setCellWidget(3,1,statusTest7);
    ui->tableWidget->setCellWidget(3,4,statusTest8);

    statusMore1 = new StatusMore();
    statusMore2 = new StatusMore();
    statusMore3 = new StatusMore();
    statusMore4 = new StatusMore();
    statusMore5 = new StatusMore();
    statusMore6 = new StatusMore();
    statusMore7 = new StatusMore();
    statusMore8 = new StatusMore();


    ui->tableWidget->setCellWidget(0,2,statusMore1);
    ui->tableWidget->setCellWidget(0,5,statusMore2);
    ui->tableWidget->setCellWidget(1,2,statusMore3);
    ui->tableWidget->setCellWidget(1,5,statusMore4);
    ui->tableWidget->setCellWidget(2,2,statusMore5);
    ui->tableWidget->setCellWidget(2,5,statusMore6);
    ui->tableWidget->setCellWidget(3,2,statusMore7);
    ui->tableWidget->setCellWidget(3,5,statusMore8);

    statusTest1->set_status(TESTING);
    statusTest2->set_status(NORMAL);
    statusTest3->set_status(NORMAL);
    statusTest4->set_status(NORMAL);
    statusTest5->set_status(NORMAL);
    statusTest6->set_status(NORMAL);
    statusTest7->set_status(NORMAL);
    statusTest8->set_status(NORMAL);


    statusMore1->setEnabled(false);
    statusMore2->setEnabled(false);
    statusMore3->setEnabled(false);
    statusMore4->setEnabled(false);
    statusMore5->setEnabled(false);
    statusMore6->setEnabled(false);
    statusMore7->setEnabled(false);
    statusMore8->setEnabled(false);
    isFailed = false;
}

/*******************************************************************************
* Function Name  :  connect_init()
* Description    :  信号槽连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTest::connect_init()
{
    connect(statusMore1, SIGNAL(clicked()), this, SLOT(slot_testCPU_debugInfo()));
    connect(statusMore2, SIGNAL(clicked()), this, SLOT(slot_testRTC_debugInfo()));
    connect(statusMore3, SIGNAL(clicked()), this, SLOT(slot_testGravity_debugInfo()));
    connect(statusMore4, SIGNAL(clicked()), this, SLOT(slot_testWiFi_debugInfo()));
    connect(statusMore5, SIGNAL(clicked()), this, SLOT(slot_testUSB_debugInfo()));
    connect(statusMore6, SIGNAL(clicked()), this, SLOT(slot_testVOL_debugInfo()));
    connect(statusMore7, SIGNAL(clicked()), this, SLOT(slot_testMIIO_debugInfo()));
    connect(statusMore8, SIGNAL(clicked()), this, SLOT(slot_syncTest_debugInfo()));
}

/*******************************************************************************
* Function Name  :  start_test
* Description    :  开始检测
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTest::start_test()
{
    statusTest1->set_status(TESTING);
    statusTest2->set_status(NORMAL);
    statusTest3->set_status(NORMAL);
    statusTest4->set_status(NORMAL);
    statusTest5->set_status(NORMAL);
    statusTest6->set_status(NORMAL);
    statusTest7->set_status(NORMAL);
    statusTest8->set_status(NORMAL);

    statusMore1->setEnabled(false);
    statusMore2->setEnabled(false);
    statusMore3->setEnabled(false);
    statusMore4->setEnabled(false);
    statusMore5->setEnabled(false);
    statusMore6->setEnabled(false);
    statusMore7->setEnabled(false);
    statusMore8->setEnabled(false);
}

/*******************************************************************************
* Function Name  :  slot_testRTC_result(int)
* Description    :  测试RTC的结果
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTest::slot_testCPU_result(int result, QString debugInfo)
{
    debugInfoCPU = debugInfo;
    if(0 == result)
    {
        statusTest1->set_status(SUCCESS);
    }
    else
    {
        isFailed = true;
        statusTest1->set_status(FAILED);
    }
    statusMore1->setEnabled(true);
    statusTest2->set_status(TESTING);
}

/*******************************************************************************
* Function Name  :  slot_testRTC_result
* Description    :  测试RTC结果
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTest::slot_testRTC_result(int result,QString debugInfo)
{
    debugInfoRTC = debugInfo;
    if(0 == result)
    {
        // 测试数据成功
        statusTest2->set_status(SUCCESS);
    }
    else
    {
        isFailed = true;
        statusTest2->set_status(FAILED);
    }
    statusMore2->setEnabled(true);
    statusTest3->set_status(TESTING);
}

/*******************************************************************************
* Function Name  :  slot_testGravity_result
* Description    :  测试G-Sensor结果
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTest::slot_testGravity_result(int result, QString debugInfo)
{
    debugInfoGravity = debugInfo;
    if(0 == result)
    {
        // 测试数据成功
        statusTest3->set_status(SUCCESS);
    }
    else
    {
        isFailed = true;
        statusTest3->set_status(FAILED);
    }
    statusMore3->setEnabled(true);
    statusTest4->set_status(TESTING);
}

/*******************************************************************************
* Function Name  :  slot_testWiFi_result
* Description    :  测试WiFi的结果
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTest::slot_testWiFi_result(int result, QString debugInfo)
{
    debugInfoWiFi = debugInfo;
    if(0 == result)
    {
        // 测试数据成功
        statusTest4->set_status(SUCCESS);
    }
    else
    {
        isFailed = true;
        statusTest4->set_status(FAILED);
    }
    statusMore4->setEnabled(true);
    statusTest5->set_status(TESTING);
}

/*******************************************************************************
* Function Name  :  slot_testUSB_result
* Description    :  测试USB_VBUS结果
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTest::slot_testUSB_result(int result, QString debugInfo)
{
    debugInfoUSB = debugInfo;
    if(0 == result)
    {
        // 测试数据成功
        statusTest5->set_status(SUCCESS);
    }
    else
    {
        isFailed = true;
        statusTest5->set_status(FAILED);
    }
    statusMore5->setEnabled(true);
    statusTest6->set_status(TESTING);
}

/*******************************************************************************
* Function Name  :  slot_testVOL_result
* Description    :  测试VOL结果
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTest::slot_testVOL_result(int result, QString debugInfo)
{
    debugInfoVOL = debugInfo;
    if(0 == result)
    {
        // 测试数据成功
        statusTest6->set_status(SUCCESS);
    }
    else
    {
        isFailed = true;
        statusTest6->set_status(FAILED);
    }

    statusMore6->setEnabled(true);
    if(!isFailed)
    {
        statusTest7->set_status(TESTING);
    }
    else
    {
        statusTest8->set_status(TESTING);
    }
}

/*******************************************************************************
* Function Name  :  slot_testMIIO_result
* Description    :  设置MIIO结果
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTest::slot_testMIIO_result(int result, QString debugInfo)
{
    debugInfoMIIO = debugInfo;
    statusMore7->setEnabled(true);
    if(0 == result)
    {
        // 测试数据成功
        statusTest7->set_status(SUCCESS);
        statusTest8->set_status(TESTING);
    }
    else
    {
        statusTest7->set_status(FAILED);
    }
}

/*******************************************************************************
* Function Name  :  slot_syncTest_result
* Description    :  上传测试数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTest::slot_syncTest_result(int result, QString debugInfo)
{
    debugInfoSync = debugInfo;
    statusMore8->setEnabled(true);
    if(0 == result)
    {
        statusTest8->set_status(SUCCESS);
    }
    else
    {
        statusTest8->set_status(FAILED);
    }
}

/*******************************************************************************
* Function Name  :  slot_testCPU_debugInfo
* Description    :  测试CPU调试信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTest::slot_testCPU_debugInfo()
{
    widgetDebugInfo->set_debugTitle(QStringLiteral("检测CPU详情"));
    widgetDebugInfo->set_debugInfo(debugInfoCPU);
    widgetDebugInfo->show();
}

/*******************************************************************************
* Function Name  :  slot_testRTC_debugInfo
* Description    :  测试RTC调试信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTest::slot_testRTC_debugInfo()
{
    widgetDebugInfo->set_debugTitle(QStringLiteral("检测RTC详情"));
    widgetDebugInfo->set_debugInfo(debugInfoRTC);
    widgetDebugInfo->show();
}

/*******************************************************************************
* Function Name  :  slot_testGravity_debugInfo
* Description    :  测试G-Sensor调试信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTest::slot_testGravity_debugInfo()
{
    widgetDebugInfo->set_debugTitle(QStringLiteral("检测G-Sensor详情"));
    widgetDebugInfo->set_debugInfo(debugInfoGravity);
    widgetDebugInfo->show();
}

/*******************************************************************************
* Function Name  :  slot_testWiFi_debugInfo
* Description    :  测试WiFi调试信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTest::slot_testWiFi_debugInfo()
{
    widgetDebugInfo->set_debugTitle(QStringLiteral("检测WiFi详情"));
    widgetDebugInfo->set_debugInfo(debugInfoWiFi);
    widgetDebugInfo->show();
}

/*******************************************************************************
* Function Name  :  slot_testUSB_debugInfo
* Description    :  测试USB调试信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTest::slot_testUSB_debugInfo()
{
    widgetDebugInfo->set_debugTitle(QStringLiteral("检测USB_VBUS详情"));
    widgetDebugInfo->set_debugInfo(debugInfoUSB);
    widgetDebugInfo->show();
}

/*******************************************************************************
* Function Name  :  slot_testVOL_debugInfo
* Description    :  测试电压调试信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTest::slot_testVOL_debugInfo()
{
    widgetDebugInfo->set_debugTitle(QStringLiteral("检测电压值详情"));
    widgetDebugInfo->set_debugInfo(debugInfoVOL);
    widgetDebugInfo->show();
}

/*******************************************************************************
* Function Name  :  slot_testMIIO_debugInfo
* Description    :  写入米家配置调试信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTest::slot_testMIIO_debugInfo()
{
    widgetDebugInfo->set_debugTitle(QStringLiteral("配置米家详情"));
    widgetDebugInfo->set_debugInfo(debugInfoMIIO);
    widgetDebugInfo->show();
}

/*******************************************************************************
* Function Name  :  slot_syncTest_debugInfo
* Description    :  上传测试数据调试信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTest::slot_syncTest_debugInfo()
{
    widgetDebugInfo->set_debugTitle(QStringLiteral("上报服务器详情"));
    widgetDebugInfo->set_debugInfo(debugInfoSync);
    widgetDebugInfo->show();
}

