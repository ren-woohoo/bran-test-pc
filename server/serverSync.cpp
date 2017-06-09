#include "serverSync.h"

/*******************************************************************************
* Function Name  :  ServerSync
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ServerSync::ServerSync(QObject *parent) : QObject(parent)
{

}

/*******************************************************************************
* Function Name  :  slot_sync_testPassed
* Description    :  同步测试结果
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerSync::slot_sync_testPassed(InfoTest infoTest)
{
    qDebug()<<"sync test~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    QNetworkRequest request;
    NetworkManager *networkManage = new NetworkManager();
    connect(networkManage, SIGNAL(signal_request_success(QString)), this, SLOT(slot_syncTest_success(QString)));
    connect(networkManage, SIGNAL(signal_request_timeout()), this, SLOT(slot_syncTest_timeout()));
    connect(networkManage, SIGNAL(signal_request_failed(QString)), this, SLOT(slot_syncTest_failed(QString)));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("Username", name.toLatin1());
    request.setRawHeader("User-Session", session.toLatin1());
    QString timestamp = QString::number(QDateTime::currentDateTime().toTime_t());
    QString encrypted = networkManage->encrypted_string(timestamp);
    request.setRawHeader("Timestamp", timestamp.toLatin1());
    request.setRawHeader("Encrypted", encrypted.toLatin1());
#ifdef TEST_MODE
    request.setRawHeader("test-db", "1");
#endif
    QString str = QString(URL_SERVER) + QString(REQUEST_SYNC_TEST_PASSED) + QString("/%1?sn=%2").arg(infoTest.infoMIIO.mac).arg(infoTest.sn);
    QUrl url = QUrl::fromEncoded(str.toLatin1());
    request.setUrl(url);

    QJsonObject test;
    QJsonObject obj;
    obj.insert("sn",infoTest.sn);
    obj.insert("did",infoTest.infoMIIO.did);
    obj.insert("mac",infoTest.infoMIIO.mac);
    obj.insert("key",infoTest.infoMIIO.key);
    obj.insert("cpu",infoTest.infoResult.cpu);
    obj.insert("rtc",infoTest.infoResult.rtc);
    obj.insert("gravity",infoTest.infoResult.gravity);
    obj.insert("wifi",infoTest.infoResult.wifi);
    obj.insert("usb",infoTest.infoResult.usb);
    obj.insert("vol",infoTest.infoResult.vol);
    obj.insert("miio",infoTest.infoResult.miio);
//    obj.insert("vol_LED",infoTest.infoVol.vol0_LED);
//    obj.insert("vol_TVOC",infoTest.infoVol.vol1_TVOC);
//    obj.insert("vol_AVCC",infoTest.infoVol.vol2_AVCC);
//    obj.insert("vol_WIFI",infoTest.infoVol.vol3_WIFI);
//    obj.insert("vol_RTC",infoTest.infoVol.vol4_RTC);
//    obj.insert("vol_IPSOUT",infoTest.infoVol.vol5_IPSOUT);
//    obj.insert("vol_DRAM",infoTest.infoVol.vol6_DRAM);
//    obj.insert("vol_5V",infoTest.infoVol.vol7_5V);
    test.insert("test_data",obj);
    QJsonDocument document;
    document.setObject(test);
    qDebug()<<"##################"<<document.toJson();
    networkManage->post_request(request,document.toJson());
}

/*******************************************************************************
* Function Name  :  slot_sync_testFailed
* Description    :  同步测试失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerSync::slot_sync_testFailed(InfoTest infoTest)
{
    qDebug()<<"sync test~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    QNetworkRequest request;
    NetworkManager *networkManage = new NetworkManager();
    connect(networkManage, SIGNAL(signal_request_success(QString)), this, SLOT(slot_syncTest_success(QString)));
    connect(networkManage, SIGNAL(signal_request_timeout()), this, SLOT(slot_syncTest_timeout()));
    connect(networkManage, SIGNAL(signal_request_failed(QString)), this, SLOT(slot_syncTest_failed(QString)));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("Username", name.toLatin1());
    request.setRawHeader("User-Session", session.toLatin1());
    QString timestamp = QString::number(QDateTime::currentDateTime().toTime_t());
    QString encrypted = networkManage->encrypted_string(timestamp);
    request.setRawHeader("Timestamp", timestamp.toLatin1());
    request.setRawHeader("Encrypted", encrypted.toLatin1());
#ifdef TEST_MODE
    request.setRawHeader("test-db", "1");
#endif
    QString str = QString(URL_SERVER) + QString(REQUEST_SYNC_TEST_FAILED) + QString("?sn=%1").arg(infoTest.sn);
    QUrl url = QUrl::fromEncoded(str.toLatin1());
    request.setUrl(url);

    QJsonObject test;
    QJsonObject obj;
    obj.insert("sn",infoTest.sn);
    obj.insert("cpu",infoTest.infoResult.cpu);
    obj.insert("rtc",infoTest.infoResult.rtc);
    obj.insert("gravity",infoTest.infoResult.gravity);
    obj.insert("wifi",infoTest.infoResult.wifi);
    obj.insert("usb",infoTest.infoResult.usb);
    obj.insert("vol",infoTest.infoResult.vol);
    obj.insert("miio",infoTest.infoResult.miio);
    obj.insert("vol_LED",infoTest.infoVol.vol0_LED);
    obj.insert("vol_TVOC",infoTest.infoVol.vol1_TVOC);
    obj.insert("vol_AVCC",infoTest.infoVol.vol2_AVCC);
    obj.insert("vol_WIFI",infoTest.infoVol.vol3_WIFI);
    obj.insert("vol_RTC",infoTest.infoVol.vol4_RTC);
    obj.insert("vol_IPSOUT",infoTest.infoVol.vol5_IPSOUT);
    obj.insert("vol_DRAM",infoTest.infoVol.vol6_DRAM);
    obj.insert("vol_5V",infoTest.infoVol.vol7_5V);
    test.insert("test_data",obj);
    QJsonDocument document;
    document.setObject(test);
    networkManage->post_request(request,document.toJson());
}

/*******************************************************************************
* Function Name  :  slot_sync_success
* Description    :  同步测试结果成功
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerSync::slot_syncTest_success(QString replyData)
{
    qDebug()<<replyData;
    QByteArray ba = replyData.toLatin1();
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(ba, &jsonError);
    if(jsonError.error == QJsonParseError::NoError)
    {
        if(doucment.isObject())
        {
            QJsonObject obj = doucment.object();
            if(obj.contains("code"))
            {
                QJsonValue codeValue = obj.take("code");
                if(codeValue.isDouble())
                {
                    if(codeValue.toVariant().toInt() != 0)
                    {
                        emit signal_syncTest_failed(replyData);
                        return;
                    }
                    else
                    {
                        emit signal_syncTest_success();
                    }
                }
                else
                {
                    emit signal_syncTest_failed(replyData);
                    return;
                }
            }
            else
            {
                emit signal_syncTest_failed(replyData);
                return;
            }
        }
        else
        {
            emit signal_syncTest_failed(replyData);
        }
    }
    else
    {
        emit signal_syncTest_failed(replyData);
    }
}

/*******************************************************************************
* Function Name  :  slot_sync_failed
* Description    :  同步测试结果失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerSync::slot_syncTest_failed(QString replyData)
{
    emit signal_syncTest_failed(replyData);
}

/*******************************************************************************
* Function Name  :  slot_sync_failed
* Description    :  同步失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerSync::slot_syncTest_timeout()
{
    emit signal_syncTest_failed("TIME OUT");
}

/*******************************************************************************
* Function Name  :  slot_update_user
* Description    :  session
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerSync::slot_update_user(QString name,QString session)
{
    this->name = name;
    this->session = session;
}
