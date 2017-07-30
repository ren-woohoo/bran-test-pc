#include "widgetDebugInfo.h"
#include "ui_widgetDebugInfo.h"

/*******************************************************************************
* Function Name  :  WidgetDebugInfo
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WidgetDebugInfo::WidgetDebugInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetDebugInfo)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    ui->stackedWidget->setCurrentWidget(ui->page_normal);
}

/*******************************************************************************
* Function Name  :  ~WidgetDebugInfo
* Description    :  析构函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WidgetDebugInfo::~WidgetDebugInfo()
{
    delete ui;
}

/*******************************************************************************
* Function Name  :  set_debugInfo
* Description    :  设置调试信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetDebugInfo::set_debugInfo(QString debugInfo)
{
    ui->textBrowser_debugInfo->setText(debugInfo);
    this->debugInfo = debugInfo;
}

/*******************************************************************************
* Function Name  :  set_debugTitle
* Description    :  设置调试标题
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetDebugInfo::set_debugTitle(QString title)
{
    ui->label->setText(title);
    this->debugTitle = title;
}

/*******************************************************************************
* Function Name  :  mousePressEvent
* Description    :  鼠标按下事件
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetDebugInfo::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton)
    {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

/*******************************************************************************
* Function Name  :  mouseMoveEvent
* Description    :  鼠标移动事件
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetDebugInfo::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

/*******************************************************************************
* Function Name  :  on_button_save_clicked()
* Description    :  保存调试信息文件
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetDebugInfo::on_button_save_clicked()
{
    QDir *debug = new QDir;
    bool exist = debug->exists("./debugInfo");
    if(!exist)
    {
        debug->mkdir("./debugInfo");
    }
    QFile file(QString("./debugInfo/%1_%2.txt").arg(debugTitle).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss")));
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream in(&file);
        in<<debugInfo;
        file.close();
        ui->stackedWidget->setCurrentWidget(ui->page_success);
    }
    else
    {
        qDebug()<<"写入文件失败";
        ui->stackedWidget->setCurrentWidget(ui->page_failed);
    }
}

/*******************************************************************************
* Function Name  :  on_textBrowser_debugInfo_textChanged()
* Description    :  调试信息改变
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetDebugInfo::on_textBrowser_debugInfo_textChanged()
{
    ui->stackedWidget->setCurrentWidget(ui->page_normal);
}

void WidgetDebugInfo::on_button_close_clicked()
{
    this->close();
}
