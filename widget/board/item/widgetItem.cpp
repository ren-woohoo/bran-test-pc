#include "widgetTestItem.h"
#include "ui_widgetTestItem.h"

/*******************************************************************************
* Function Name  :  WidgetTestItem
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WidgetTestItem::WidgetTestItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTestItem)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->page_free);
    data_init();
    connect_init();
}

/*******************************************************************************
* Function Name  :  ~WidgetTestItem
* Description    :  析构函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WidgetTestItem::~WidgetTestItem()
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
void WidgetTestItem::data_init()
{
    status = FREE;
    testControl = new TestControl();
    testControl->start();

    widgetTest = new WidgetTest();

    timerProgress = new QTimer();
    timerProgress->setInterval(500);
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTestItem::connect_init()
{
    connect(this, SIGNAL(signal_start_test()), testControl, SLOT(slot_start_test()));
    connect(this, SIGNAL(signal_stop_test()), testControl, SLOT(slot_stop_test()));
    connect(this, SIGNAL(signal_set_device(QString)), testControl, SLOT(slot_set_deviceADB(QString)));
    connect(this, SIGNAL(signal_connect_fixture(QString)), testControl, SLOT(slot_connect_fixture(QString)));

    connect(this, SIGNAL(signal_get_device()), testControl, SLOT(slot_get_device()));

    connect(testControl, SIGNAL(signal_update_infoFixture(InfoFixture)), this, SLOT(slot_update_infoFixture(InfoFixture)));

    connect(testControl, SIGNAL(signal_update_progress(int)), this, SLOT(slot_update_progress(int)));

    connect(timerProgress, SIGNAL(timeout()), this, SLOT(slot_refresh_progress()));

    connect(testControl, SIGNAL(signal_test_result(int)), this, SLOT(slot_test_result(int)));

    connect(testControl, SIGNAL(signal_testCPU_result(int,QString)), widgetTest, SLOT(slot_testCPU_result(int,QString)));
    connect(testControl, SIGNAL(signal_testRTC_result(int,QString)), widgetTest, SLOT(slot_testRTC_result(int,QString)));
    connect(testControl, SIGNAL(signal_testGravity_result(int,QString)), widgetTest, SLOT(slot_testGravity_result(int,QString)));
    connect(testControl, SIGNAL(signal_testWiFi_result(int,QString)), widgetTest, SLOT(slot_testWiFi_result(int,QString)));
    connect(testControl, SIGNAL(signal_testUSB_result(int,QString)), widgetTest, SLOT(slot_testUSB_result(int,QString)));
    connect(testControl, SIGNAL(signal_testVOL_result(int,QString)), widgetTest, SLOT(slot_testVOL_result(int,QString)));
    connect(testControl, SIGNAL(signal_testMIIO_result(int,QString)), widgetTest, SLOT(slot_testMIIO_result(int,QString)));
    connect(testControl, SIGNAL(signal_syncTest_result(int,QString)), widgetTest, SLOT(slot_syncTest_result(int,QString)));
}

/*******************************************************************************
* Function Name  :  page_init
* Description    :  页面初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTestItem::page_init()
{

}

/*******************************************************************************
* Function Name  :  start_test
* Description    :  开始检测
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTestItem::start_test()
{
    status = STAGE_TESTING;
    widgetTest->start_test();
    ui->progressBar->setValue(0);
    progress = 0;
    timerProgress->start();
    emit signal_start_test();
    ui->button_more->setText("TEST...");
    ui->button_more->setStyleSheet("background-color:white;color:#424242");

    // 跳转到检测页面
    ui->stackedWidget->setCurrentWidget(ui->page_testing);
}

/*******************************************************************************
* Function Name  :  slot_test_result
* Description    :  测试结果
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTestItem::slot_test_result(int result)
{
    if(STAGE_TESTING == status)
    {
        status = STAGE_TESTED;
    }
    emit signal_test_end();
    if(0 == result)
    {
        ui->button_more->setText("PASS");
        ui->button_more->setStyleSheet("background-color:#00e676;color:white");
    }
    else
    {
        ui->button_more->setText("FAIL");
        ui->button_more->setStyleSheet("background-color:#f44336;color:white");
    }
}

/*******************************************************************************
* Function Name  :  slot_update_progress
* Description    :  刷新进度
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTestItem::slot_update_progress(int progress)
{
    this->progress = progress;
//    ui->progressBar->setValue(progress);
}

/*******************************************************************************
* Function Name  :  slot_refresh_progress
* Description    :  刷新进度条
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTestItem::slot_refresh_progress()
{
    QEventLoop loop;
    int  add = (progress - ui->progressBar->value())/10;
    for(int i = 0; i < 10; ++i)
    {
        ui->progressBar->setValue(ui->progressBar->value() + add);
        QTimer::singleShot(100, &loop, SLOT(quit()));
        loop.exec();
    }
}

/*******************************************************************************
* Function Name  :  get_status
* Description    :  获取状态
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
TestStage WidgetTestItem::get_status()
{
    return  status;
}

/*******************************************************************************
* Function Name  :  load_fixture
* Description    :  加载治具
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTestItem::load_fixture(QString port)
{
    status = LOADED;
    this->port = port;
    ui->label_port->setText(port);
    emit signal_connect_fixture(port);
    ui->stackedWidget->setCurrentWidget(ui->page_index);
}

/*******************************************************************************
* Function Name  :  remove_fixture
* Description    :  移除治具
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTestItem::remove_fixture()
{
    status = FREE;
    port = "";
    ui->groupBox->setTitle("治具SN");
    ui->stackedWidget->setCurrentWidget(ui->page_free);
}

/*******************************************************************************
* Function Name  :  get_fixture
* Description    :  获取治具串口号
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString WidgetTestItem::get_fixture()
{
    return port;
}

/*******************************************************************************
* Function Name  :  load_device
* Description    :  载入设备
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTestItem::load_device(QString device)
{
    status = MATCHED;
    this->device = device;
    emit signal_set_device(device);
    ui->label_device->setText(device);
}

/*******************************************************************************
* Function Name  :  remove_device
* Description    :  移除设备
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTestItem::remove_device()
{
    qDebug()<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";
    status = CONNECTED;
    ui->label_device->setText("UNKNOWN");
    ui->label_deviceSN->setText("UNKNOWN");
    ui->stackedWidget->setCurrentWidget(ui->page_index);
}

/*******************************************************************************
* Function Name  :  get_device
* Description    :  获取设备号
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString WidgetTestItem::get_device()
{
    return device;
}

QString WidgetTestItem::get_deviceSN()
{
    return deviceSN;
}

/*******************************************************************************
* Function Name  :  sample_device
* Description    :  采集设备
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTestItem::sample_device()
{
    if(status == READY)
    {
        emit signal_match_device(deviceSN);
    }
    else
    {
        emit signal_sample_deviceSN();
    }
}

/*******************************************************************************
* Function Name  :  slot_update_fixtureSN
* Description    :  刷新治具信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTestItem::slot_update_fixtureSN(QString fixtureSN)
{
    if(status == READY)
    {
        emit signal_match_device(deviceSN);
    }
    else
    {
        if(status < CONNECTED)
        {
            status = CONNECTED;
        }
        qDebug()<<"#################################"<<fixtureSN<<ui->groupBox->width();
        this->fixtureSN = fixtureSN;
        QFontMetrics elideFont(ui->label_deviceSN->font());
        ui->groupBox->setTitle(elideFont.elidedText(QString("治具UID:%1").arg(fixtureSN), Qt::ElideRight, 200));
    }
}

/*******************************************************************************
* Function Name  :  slot_update_deviceSN
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTestItem::slot_update_deviceSN(QString deviceSN)
{
    this->deviceSN = deviceSN;
    QFontMetrics elideFont(ui->label_deviceSN->font());
    ui->label_deviceSN->setText(elideFont.elidedText(deviceSN, Qt::ElideRight, ui->label_deviceSN->width()));
    if(status < MATCHED)
    {
        status = READY;
        emit signal_match_device(deviceSN);
    }
}

/*******************************************************************************
* Function Name  :  on_button_more_clicked()
* Description    :  显示具体检测信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTestItem::on_button_more_clicked()
{
    widgetTest->show();
}
