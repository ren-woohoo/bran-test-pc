#include "statusTest.h"
#include "ui_statusTest.h"

StatusTest::StatusTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusTest)
{
    ui->setupUi(this);
    set_status(NORMAL);
}

/*******************************************************************************
* Function Name  :  ~StatusTest
* Description    :  析构函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
StatusTest::~StatusTest()
{
    delete ui;
}

/*******************************************************************************
* Function Name  :  set_status
* Description    :  设置状态
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void StatusTest::set_status(TestStatusType testStatus)
{
    switch (testStatus)
    {
        case SUCCESS:
            ui->stackedWidget->setCurrentWidget(ui->page_success);
            break;
        case FAILED:
            ui->stackedWidget->setCurrentWidget(ui->page_failed);
            break;
        case TESTING:
            ui->stackedWidget->setCurrentWidget(ui->page_testing);
            break;
        default:
            ui->stackedWidget->setCurrentWidget(ui->page_normal);
            break;
    }

}
