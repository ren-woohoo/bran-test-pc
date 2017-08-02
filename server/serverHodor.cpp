#include "serverHodor.h"

/*******************************************************************************
* Function Name  :  ServerHodor
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ServerHodor::ServerHodor(QObject *parent) : QObject(parent)
{

}

/*******************************************************************************
* Function Name  :  slot_sync_testPassed
* Description    :  同步测试结果
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerHodor::slot_sync_testPassed(InfoDevice infoDevice,InfoHodor infoHodor)
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
    request.setRawHeader("Version", QString(VERSION).toLatin1());
    request.setRawHeader("Api-Version", QString("1").toLatin1());
    request.setRawHeader("Client", QString("pc").toLatin1());
    request.setRawHeader("Package", QString("hodor").toLatin1());
#ifdef TEST_MODE
    request.setRawHeader("test-db", "1");
#endif
    QString str = QString(URL_SERVER) + QString(REQUEST_HODOR_TEST_PASS) + QString("/%1").arg(infoDevice.infoMiio.mac);
    QUrl url = QUrl::fromEncoded(str.toLatin1());
    request.setUrl(url);

    QJsonObject test;
    QJsonObject obj;
    obj.insert("sn",infoDevice.deviceSN);
    obj.insert("did",infoDevice.infoMiio.did);
    obj.insert("mac",infoDevice.infoMiio.mac);
    obj.insert("key",infoDevice.infoMiio.key);
    obj.insert("pm25",infoHodor.pm25);
    obj.insert("temp_humi",infoHodor.tempHumi);
    obj.insert("tvoc_co2e",infoHodor.tvocCo2e);
    obj.insert("gravity",infoHodor.gravity);
    obj.insert("light",infoHodor.light);
    obj.insert("battery",infoHodor.battery);
    obj.insert("wifi",infoHodor.wifi);
    obj.insert("button",infoHodor.button);
    obj.insert("touch",infoHodor.touch);
    test.insert("test_data",obj);
    QJsonDocument document;
    document.setObject(test);
    requestData = QString("url:%1\ndata:%2").arg(str).arg(QString(document.toJson()));
    networkManage->post_request(request,document.toJson());
}

/*******************************************************************************
* Function Name  :  slot_sync_testFailed
* Description    :  同步测试失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerHodor::slot_sync_testFailed(InfoDevice infoDevice,InfoHodor infoHodor)
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
    request.setRawHeader("Version", QString(VERSION).toLatin1());
    request.setRawHeader("Api-Version", QString("1").toLatin1());
    request.setRawHeader("Client", QString("pc").toLatin1());
    request.setRawHeader("Package", QString("hodor").toLatin1());
#ifdef TEST_MODE
    request.setRawHeader("test-db", "1");
#endif
    QString str = QString(URL_SERVER) + QString(REQUEST_HODOR_TEST_FAILED) + QString("/%1").arg(infoDevice.infoMiio.mac);
    QUrl url = QUrl::fromEncoded(str.toLatin1());
    request.setUrl(url);

    QJsonObject test;
    QJsonObject obj;
    obj.insert("sn",infoDevice.deviceSN);
    obj.insert("did",infoDevice.infoMiio.did);
    obj.insert("mac",infoDevice.infoMiio.mac);
    obj.insert("key",infoDevice.infoMiio.key);
    obj.insert("pm25",infoHodor.pm25);
    obj.insert("temp_humi",infoHodor.tempHumi);
    obj.insert("tvoc_co2e",infoHodor.tvocCo2e);
    obj.insert("gravity",infoHodor.gravity);
    obj.insert("light",infoHodor.light);
    obj.insert("battery",infoHodor.battery);
    obj.insert("wifi",infoHodor.wifi);
    obj.insert("button",infoHodor.button);
    obj.insert("touch",infoHodor.touch);
    test.insert("test_data",obj);
    QJsonDocument document;
    document.setObject(test);
    requestData = QString("url:%1\ndata:%2").arg(str).arg(QString(document.toJson()));
    networkManage->post_request(request,document.toJson());
}

/*******************************************************************************
* Function Name  :  slot_sync_success
* Description    :  同步测试结果成功
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerHodor::slot_syncTest_success(QString replyData)
{
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
                        emit signal_syncTest_failed(requestData, replyData);
                        return;
                    }
                    else
                    {
                        emit signal_syncTest_success(requestData, replyData);
                    }
                }
                else
                {
                    emit signal_syncTest_failed(requestData, replyData);
                    return;
                }
            }
            else
            {
                emit signal_syncTest_failed(requestData, replyData);
                return;
            }
        }
        else
        {
            emit signal_syncTest_failed(requestData, replyData);
        }
    }
    else
    {
        emit signal_syncTest_failed(requestData, replyData);
    }
}

/*******************************************************************************
* Function Name  :  slot_sync_failed
* Description    :  同步测试结果失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerHodor::slot_syncTest_failed(QString replyData)
{
    emit signal_syncTest_failed(requestData, replyData);
}

/*******************************************************************************
* Function Name  :  slot_sync_failed
* Description    :  同步失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerHodor::slot_syncTest_timeout()
{
    emit signal_syncTest_failed(requestData, "TIME OUT");
}

/*******************************************************************************
* Function Name  :  slot_update_user
* Description    :  session
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerHodor::slot_update_user(QString name,QString session)
{
    this->name = name;
    this->session = session;
}
