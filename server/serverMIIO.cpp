#include "serverMIIO.h"

/*******************************************************************************
* Function Name  :  ServerMac
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ServerMIIO::ServerMIIO(QObject *parent) : QObject(parent)
{

}

/*******************************************************************************
* Function Name  :  slot_fetch_mac()
* Description    :  获取MAC
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerMIIO::slot_fetch_mac(QString sn)
{
    QNetworkRequest request;
    NetworkManager *networkManage = new NetworkManager();
    connect(networkManage, SIGNAL(signal_request_success(QString)), this, SLOT(slot_fetch_success(QString)));
    connect(networkManage, SIGNAL(signal_request_timeout()), this, SLOT(slot_fetch_timeout()));
    connect(networkManage, SIGNAL(signal_request_failed(QString)), this, SLOT(slot_fetch_failed(QString)));

    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    request.setRawHeader("Username", name.toLatin1());
    qDebug()<<session;
    request.setRawHeader("User-Session", session.toLatin1());
    QString timestamp = QString::number(QDateTime::currentDateTime().toTime_t());
    QString encrypted = networkManage->encrypted_string(timestamp);
    request.setRawHeader("Timestamp", timestamp.toLatin1());
    request.setRawHeader("Encrypted", encrypted.toLatin1());
    request.setRawHeader("Version", QString(VERSION).toLatin1());
    request.setRawHeader("Api-Version", QString("1").toLatin1());
    request.setRawHeader("Client", QString("pc").toLatin1());
    request.setRawHeader("Package", QString("bran").toLatin1());
#ifdef TEST_MODE
    request.setRawHeader("test-db", "1");
#endif
    QString str = QString(URL_SERVER) + QString(REQUEST_FETCH_MAC)+QString("?sn=%1").arg(sn);
    QUrl urlMac = QUrl::fromEncoded(str.toLatin1());
    request.setUrl(urlMac);
    networkManage->get_request(request);
}

/*******************************************************************************
* Function Name  :  slot_fetch_success
* Description    :  寻求MAC
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerMIIO::slot_fetch_success(QString replyData)
{
    qDebug()<<"replyData"<<replyData;
    QByteArray ba = replyData.toLatin1();
    QJsonParseError jsonError;
    InfoMIIO infoMIIO;
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
                        emit signal_fetch_failed(replyData);
                        return;
                    }
                }
                else
                {
                    emit signal_fetch_failed(replyData);
                    return;
                }
            }
            else
            {
                emit signal_fetch_failed(replyData);
                return;
            }

            if(obj.contains("address"))
            {
                QJsonValue macValue = obj.take("address");
                if(macValue.isString())
                {
                    infoMIIO.mac = macValue.toString();
                }
                else
                {
                    emit signal_fetch_failed(replyData);
                    return;
                }
            }
            else
            {
                emit signal_fetch_failed(replyData);
                return;
            }

            if(obj.contains("did"))
            {
                QJsonValue didValue = obj.take("did");
                if(didValue.isString())
                {
                    infoMIIO.did = didValue.toString();
                }
                else
                {
                    emit signal_fetch_failed(replyData);
                    return;
                }
            }
            else
            {
                emit signal_fetch_failed(replyData);
                return;
            }

            if(obj.contains("key"))
            {
                QJsonValue keyValue = obj.take("key");
                if(keyValue.isString())
                {
                    infoMIIO.key = keyValue.toString();
                }
                else
                {
                    emit signal_fetch_failed(replyData);
                    return;
                }
            }
            else
            {
                emit signal_fetch_failed(replyData);
                return;
            }
            emit signal_fetch_success(infoMIIO);
        }
        else
        {
            emit signal_fetch_failed(replyData);
            return;
        }
    }
    else
    {
        emit signal_fetch_failed(replyData);
        return;
    }
}

/*******************************************************************************
* Function Name  :  slot_fetch_failed
* Description    :  获取MAC地址失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerMIIO::slot_fetch_failed(QString replyData)
{
    emit signal_fetch_failed(replyData);
}

void ServerMIIO::slot_fetch_timeout()
{
    emit signal_fetch_failed("TIME OUT");
}

/*******************************************************************************
* Function Name  :  slot_update_user
* Description    :  session
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerMIIO::slot_update_user(QString name,QString session)
{
    this->name = name;
    this->session = session;
}
