#include "widgetHodor.h"
#include "ui_widgetHodor.h"

WidgetHodor::WidgetHodor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetHodor)
{
    ui->setupUi(this);
    data_init();
    connect_init();
}

WidgetHodor::~WidgetHodor()
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
void WidgetHodor::data_init()
{
    hodorControl = HodorControl::getInstance();
    hodorControl->start();
    statusTest1 = new StatusTest();
    statusTest2 = new StatusTest();
    statusTest3 = new StatusTest();
    statusTest4 = new StatusTest();
    statusTest5 = new StatusTest();
    statusTest6 = new StatusTest();
    statusTest7 = new StatusTest();
    statusTest8 = new StatusTest();
    statusTest9 = new StatusTest();

    statusTest1->setGeometry(0,0,150,60);
    statusTest2->setGeometry(0,0,150,60);
    statusTest3->setGeometry(0,0,150,60);
    statusTest4->setGeometry(0,0,150,60);
    statusTest5->setGeometry(0,0,150,60);
    statusTest6->setGeometry(0,0,150,60);
    statusTest7->setGeometry(0,0,150,60);
    statusTest8->setGeometry(0,0,150,60);
    statusTest9->setGeometry(0,0,150,60);

    ui->tableWidget->setCellWidget(0,1,statusTest1);
    ui->tableWidget->setCellWidget(0,3,statusTest2);
    ui->tableWidget->setCellWidget(1,1,statusTest3);
    ui->tableWidget->setCellWidget(1,3,statusTest4);
    ui->tableWidget->setCellWidget(2,1,statusTest5);
    ui->tableWidget->setCellWidget(2,3,statusTest6);
    ui->tableWidget->setCellWidget(3,1,statusTest7);
    ui->tableWidget->setCellWidget(3,3,statusTest8);
    ui->tableWidget->setCellWidget(4,1,statusTest9);

    statusTest1->set_status(NORMAL);
    statusTest2->set_status(NORMAL);
    statusTest3->set_status(NORMAL);
    statusTest4->set_status(NORMAL);
    statusTest5->set_status(NORMAL);
    statusTest6->set_status(NORMAL);
    statusTest7->set_status(NORMAL);
    statusTest8->set_status(NORMAL);
    statusTest9->set_status(NORMAL);
    ui->button_sync->setEnabled(false);
    ui->stackedWidget_result->setCurrentWidget(ui->page_normal);
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽初试化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetHodor::connect_init()
{
    connect(this, SIGNAL(signal_sync_hodor()), hodorControl, SLOT(slot_sync_hodor()));

    connect(hodorControl, SIGNAL(signal_update_device(InfoDevice,InfoHodor)), this, SLOT(slot_update_device(InfoDevice,InfoHodor)));
    connect(hodorControl, SIGNAL(signal_remove_device()), this, SLOT(slot_remove_device()));
    connect(hodorControl, SIGNAL(signal_syncTest_success(QString, QString)), this, SLOT(slot_syncTest_success(QString, QString)));
    connect(hodorControl, SIGNAL(signal_syncTest_failed(QString, QString)), this, SLOT(slot_syncTest_failed(QString, QString)));
    connect(hodorControl, SIGNAL(signal_update_debugInfo(QString)), this, SLOT(slot_update_debugInfo(QString)));
}

/*******************************************************************************
* Function Name  :  slot_remove_device
* Description    :  删除设备
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetHodor::slot_remove_device()
{
    ui->edit_ack->clear();
    ui->edit_req->clear();
    ui->button_sync->setEnabled(false);
    ui->label_adb->setText("UNKNOWN");
    ui->label_did->setText("UNKNOWN");
    ui->label_key->setText("UNKNOWN");
    ui->label_mac->setText("UNKNOWN");
    ui->label_sn->setText("UNKNOWN");

    statusTest1->set_status(NORMAL);
    statusTest2->set_status(NORMAL);
    statusTest3->set_status(NORMAL);
    statusTest4->set_status(NORMAL);
    statusTest5->set_status(NORMAL);
    statusTest6->set_status(NORMAL);
    statusTest7->set_status(NORMAL);
    statusTest8->set_status(NORMAL);
    statusTest9->set_status(NORMAL);
    ui->stackedWidget_result->setCurrentWidget(ui->page_normal);
}

/*******************************************************************************
* Function Name  :  slot_update_device
* Description    :  刷新此时设备
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetHodor::slot_update_device(InfoDevice infoDevice, InfoHodor infoHodor)
{
    ui->edit_ack->clear();
    ui->edit_req->clear();
    ui->button_sync->setEnabled(true);
    ui->label_adb->setText(infoDevice.deviceADB);
    ui->label_did->setText(infoDevice.infoMiio.did);
    ui->label_key->setText(infoDevice.infoMiio.key);
    ui->label_mac->setText(infoDevice.infoMiio.mac);
    ui->label_sn->setText(infoDevice.deviceSN);
    if(infoHodor.tempHumi)
    {
        statusTest1->set_status(SUCCESS);
    }
    else
    {
        statusTest1->set_status(FAILED);
    }

    if(infoHodor.light)
    {
        statusTest2->set_status(SUCCESS);
    }
    else
    {
        statusTest2->set_status(FAILED);
    }
    if(infoHodor.gravity)
    {
        statusTest3->set_status(SUCCESS);
    }
    else
    {
        statusTest3->set_status(FAILED);
    }
    if(infoHodor.button)
    {
        statusTest4->set_status(SUCCESS);
    }
    else
    {
        statusTest4->set_status(FAILED);
    }
    if(infoHodor.touch)
    {
        statusTest5->set_status(SUCCESS);
    }
    else
    {
        statusTest5->set_status(FAILED);
    }
    if(infoHodor.wifi)
    {
        statusTest6->set_status(SUCCESS);
    }
    else
    {
        statusTest6->set_status(FAILED);
    }
    if(infoHodor.tvocCo2e)
    {
        statusTest7->set_status(SUCCESS);
    }
    else
    {
        statusTest7->set_status(FAILED);
    }
    if(infoHodor.battery)
    {
        statusTest8->set_status(SUCCESS);
    }
    else
    {
        statusTest8->set_status(FAILED);
    }
    if(infoHodor.pm25)
    {
        statusTest9->set_status(SUCCESS);
    }
    else
    {
        statusTest9->set_status(FAILED);
    }
}

/*******************************************************************************
* Function Name  :  on_button_sync_clicked()
* Description    :  上报测试数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetHodor::on_button_sync_clicked()
{
    ui->edit_req->clear();
    ui->edit_ack->clear();
    ui->stackedWidget_result->setCurrentWidget(ui->page_normal);
    emit signal_sync_hodor();
    ui->button_sync->setEnabled(false);
}

void WidgetHodor::slot_syncTest_success(QString request, QString reply)
{
    ui->edit_req->setText(request);
    ui->edit_ack->setText(reply);
    ui->stackedWidget_result->setCurrentWidget(ui->page_success);
    ui->button_sync->setEnabled(true);
}

void WidgetHodor::slot_syncTest_failed(QString request, QString reply)
{
    ui->edit_req->setText(request);
    ui->edit_ack->setText(reply);
    ui->stackedWidget_result->setCurrentWidget(ui->page_failed);
    ui->button_sync->setEnabled(true);
}

void WidgetHodor::slot_update_debugInfo(QString debug)
{
    ui->edit_req->setText(debug);
}
