#include "networkManager.h"

NetworkManager::NetworkManager()
    :QNetworkAccessManager()
{
    durationRequest = 10000;
}

/*******************************************************************************
* Function Name  :  get_request
* Description    :  GET
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void NetworkManager::get_request(QNetworkRequest request)
{

    QTimer *timer = new QTimer();
    timer->setInterval(durationRequest);
    timer->setSingleShot(true);
    QEventLoop loop;
    QString replyData;
    QNetworkReply *reply;
    QByteArray data;
    int num = 1;

    connect(timer,SIGNAL(timeout()), &loop, SLOT(quit()));
    while(num -- > 0)
    {
        reply = this->get(request);
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        timer->start();

        loop.exec();
        if (timer->isActive())
        {
            timer->stop();
            if (reply->error() != QNetworkReply::NoError)
            {
                qDebug()<<QNetworkReply::NoError;
                data = reply->readAll();
                qDebug()<< "Error String : " << reply->errorString();
                qDebug()<<data;
                replyData.prepend(data);
                emit signal_request_failed(replyData);
                return;
            }
            else
            {
                QVariant variant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                int nStatusCode = variant.toInt();
                if(nStatusCode == 200)
                {
                    data = reply->readAll();
                    replyData.prepend(data);
                    emit signal_request_success(replyData);
                    return;
                }
            }
        }
        else
        {
            disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            reply->abort();
            reply->deleteLater();
        }
    }
    emit signal_request_timeout();
}

/*******************************************************************************
* Function Name  :  post_request
* Description    :  POST
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void NetworkManager::post_request(QNetworkRequest request, QByteArray postData)
{
    QTimer *timer = new QTimer();
    timer->setInterval(durationRequest);
    timer->setSingleShot(true);

    QEventLoop loop;
    QString replyData;
    QByteArray data;
    QNetworkReply *reply;
    int num = 1;
    connect(timer,SIGNAL(timeout()), &loop, SLOT(quit()));
    while(num-- > 0)
    {
        qDebug()<<"########################################";
        reply = this->post(request,postData);
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        timer->start();

        loop.exec();

        if (timer->isActive())
        {
            timer->stop();
            if (reply->error() != QNetworkReply::NoError)
            {
                data = reply->readAll();
                qDebug()<< "Error String : " << reply->errorString();
                qDebug()<<data;
                replyData.prepend(data);
                emit signal_request_failed(replyData);
                return;
            }
            else
            {
                QVariant variant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                int nStatusCode = variant.toInt();
                data = reply->readAll();
                qDebug()<<nStatusCode<<data;
                if(nStatusCode == 200)
                {
                    replyData.prepend(data);
                    qDebug()<<replyData;

                    emit signal_request_success(replyData);
                    return;
                }
            }
        }
        else
        {
            disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            reply->abort();
            reply->deleteLater();
        }
    }
    emit signal_request_timeout();
}

/*******************************************************************************
* Function Name  :  encrypted_string
* Description    :  加密字符串
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString NetworkManager::encrypted_string(QString timestamp)
{
    QString encrypted1 = timestamp.insert(0,"000");
    encrypted1.append("000");
    QString encrypted2 = "";
    for(int i = encrypted1.length() -1; i >=0 ; --i)
    {
        encrypted2.append(encrypted1.at(i));
    }
    QString md5;
    QString md52;
    QByteArray ba;
    ba = QCryptographicHash::hash (encrypted2.toLatin1(), QCryptographicHash::Md5 );
    md5.append(ba.toHex());
    for(int j = md5.length() -1; j >=0 ; --j)
    {
        md52.append(md5.at(j));
    }
    QString encrypted = QString("V1_%1").arg(md52);
    qDebug()<<encrypted1;
    qDebug()<<encrypted2;
    qDebug()<<md5;
    qDebug()<<md52;
    return encrypted;
}

