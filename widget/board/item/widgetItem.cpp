#include "widgetItem.h"
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
    connect(timerProgress, SIGNAL(timeout()), this, SLOT(slot_refresh_progress()));
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
void WidgetTestItem::slot_test_init()
{
    progress = 0;
    ui->progressBar->setValue(0);
    timerProgress->start();
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
* Function Name  :  load_fixture
* Description    :  加载治具
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetTestItem::slot_update_infoFixture(InfoFixture infoFixture)
{
    if(infoFixture.isEmpty())
    {
        ui->groupBox->setTitle("治具SN");
        ui->label_port->setText("UNKNOWN");
        ui->label_device->setText("UNKNOWN");
        ui->label_deviceSN->setText("UNKNOWN");
        ui->stackedWidget->setCurrentWidget(ui->page_free);
    }
    else
    {
        if(infoFixture.deviceADB.isEmpty())
        {
            ui->label_device->setText("UNKNOWN");
        }
        else
        {
            ui->label_device->setText(infoFixture.deviceADB);
        }
        if(infoFixture.deviceSN.isEmpty())
        {
            ui->label_deviceSN->setText("UNKNOWN");
        }
        else
        {
            QFontMetrics elideFont(ui->label_deviceSN->font());
            ui->label_deviceSN->setText(elideFont.elidedText(infoFixture.deviceSN, Qt::ElideRight, ui->label_deviceSN->width()));
        }
        if(infoFixture.id.isEmpty())
        {
            ui->groupBox->setTitle("治具SN");
        }
        else
        {
            QFontMetrics elideFont(ui->label_deviceSN->font());
            ui->groupBox->setTitle(elideFont.elidedText(QString("治具UID:%1").arg(infoFixture.id), Qt::ElideRight, 200));
        }
        ui->label_port->setText(infoFixture.port);
        ui->stackedWidget->setCurrentWidget(ui->page_index);
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
    emit signal_show_widgetTest();
}
