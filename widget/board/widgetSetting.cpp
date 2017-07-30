#include "widgetSetting.h"
#include "ui_widgetSetting.h"

/*******************************************************************************
* Function Name  :  WidgetSetting
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WidgetSetting::WidgetSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetSetting)
{
    ui->setupUi(this);
    qRegisterMetaType<RefDatas>("RefDatas");
    data_init();
    widget_init();
    connect_init();
}

/*******************************************************************************
* Function Name  :  ~WidgetSetting
* Description    :  析构函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WidgetSetting::~WidgetSetting()
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
void WidgetSetting::data_init()
{
    sysData  = SysData::getInstance();
    refDatas = sysData->get_refDatas();
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetSetting::connect_init()
{
    connect(this, SIGNAL(signal_change_refDatas(RefDatas)), sysData, SLOT(slot_change_refDatas(RefDatas)));
}

/*******************************************************************************
* Function Name  :  widget_init
* Description    :  窗口初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetSetting::widget_init()
{
    QDoubleValidator *error = new QDoubleValidator(this);
    error->setRange(0, 50);
    error->setNotation(QDoubleValidator::StandardNotation);
    error->setDecimals(2);
    ui->edit_error->setText(QString::number(refDatas.dataError));

    QDoubleValidator *ref = new QDoubleValidator(this);
    ref->setRange(0,10);
    ref->setNotation(QDoubleValidator::StandardNotation);
    ref->setDecimals(2);

    ui->edit_5V->setValidator(ref);
    ui->edit_AVCC->setValidator(ref);
    ui->edit_DRAM->setValidator(ref);
    ui->edit_IPSOUT->setValidator(ref);
    ui->edit_LED->setValidator(ref);
    ui->edit_RTC->setValidator(ref);
    ui->edit_TVOC->setValidator(ref);
    ui->edit_WIFI->setValidator(ref);

    ui->edit_LED->setText(QString::number(refDatas.dataVol0));
    ui->edit_TVOC->setText(QString::number(refDatas.dataVol1));
    ui->edit_AVCC->setText(QString::number(refDatas.dataVol2));
    ui->edit_WIFI->setText(QString::number(refDatas.dataVol3));
    ui->edit_RTC->setText(QString::number(refDatas.dataVol4));
    ui->edit_IPSOUT->setText(QString::number(refDatas.dataVol5));
    ui->edit_DRAM->setText(QString::number(refDatas.dataVol6));
    ui->edit_5V->setText(QString::number(refDatas.dataVol7));
}

/*******************************************************************************
* Function Name  :  on_button_change_clicked()
* Description    :  修改当前合理值
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WidgetSetting::on_button_change_clicked()
{
    refDatas.dataVol0 = get_validValue(ui->edit_LED->text());
    refDatas.dataVol1 = get_validValue(ui->edit_TVOC->text());
    refDatas.dataVol2 = get_validValue(ui->edit_AVCC->text());
    refDatas.dataVol3 = get_validValue(ui->edit_WIFI->text());
    refDatas.dataVol4 = get_validValue(ui->edit_RTC->text());
    refDatas. dataVol5 = get_validValue(ui->edit_IPSOUT->text());
    refDatas.dataVol6 = get_validValue(ui->edit_DRAM->text());
    refDatas.dataVol7 = get_validValue(ui->edit_5V->text());
    refDatas.dataError = get_validValue(ui->edit_error->text());
    emit signal_change_refDatas(refDatas);
}

/*******************************************************************************
* Function Name  :  get_validValue
* Description    :  获取有效值
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
float WidgetSetting::get_validValue(QString str)
{
    if(str == "")
    {
        return ERROR_DATA;
    }
    else
    {
        return str.toFloat();
    }
}
