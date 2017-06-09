#include "statusMore.h"
#include "ui_statusMore.h"

/*******************************************************************************
* Function Name  :  StatusMore
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
StatusMore::StatusMore(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusMore)
{
    ui->setupUi(this);
    this->setEnabled(false);
}

/*******************************************************************************
* Function Name  :  ~StatusMore
* Description    :  析构函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
StatusMore::~StatusMore()
{
    delete ui;
}

/*******************************************************************************
* Function Name  :  on_button_more_clicked
* Description    :  查看详情
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void StatusMore::on_button_more_clicked()
{
    emit clicked();
}

/*******************************************************************************
* Function Name  :  setEnabled
* Description    :  设置是否可用
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void StatusMore::setEnabled(bool enabled)
{
    ui->button_more->setEnabled(enabled);
    if(enabled)
    {
        ui->stackedWidget->setCurrentWidget(ui->page_button);
    }
    else
    {
        ui->stackedWidget->setCurrentWidget(ui->page_wating);
    }
}
