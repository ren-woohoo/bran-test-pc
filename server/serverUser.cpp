#include "serverUser.h"

ServerUser *ServerUser::instance = 0;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ServerUser* ServerUser::getInstance()
{
    if( 0 == instance)
    {
        instance = new ServerUser();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  ServerUser
* Description    :
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ServerUser::ServerUser(QObject *parent) : QObject(parent)
{

}

/*******************************************************************************
* Function Name  :  slot_user_login
* Description    :  用户登录
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerUser::slot_user_login(QString name, QString key)
{
    qDebug()<<name<<key;
    this->name = name;
    QNetworkRequest request;
    NetworkManager *networkManage = new NetworkManager();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    QString password;
    QByteArray ba;
    ba = QCryptographicHash::hash (key.toLatin1(), QCryptographicHash::Md5);
    password.append(ba.toHex());
    QString str = QString(URL_SERVER).append(QString(REQUEST_LOGIN));
    QUrl urlLogin = QUrl::fromEncoded(str.toLatin1());

    qDebug()<<name<<password;
    qDebug()<<str;

    QByteArray postData;
    postData.append("username="+name+"&");
    postData.append("password="+password+"&");
    postData.append("project=bran");
    request.setUrl(urlLogin);

    connect(networkManage, SIGNAL(signal_request_success(QString)), this, SLOT(slot_login_success(QString)));
    connect(networkManage, SIGNAL(signal_request_timeout()), this, SLOT(slot_login_failed()));
    connect(networkManage, SIGNAL(signal_request_failed(QString)), this, SLOT(slot_login_failed()));
    networkManage->post_request(request,postData);
}


/*******************************************************************************
* Function Name  :  slot_login_success
* Description    :
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerUser::slot_login_success(QString replyData)
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
                        emit signal_login_result(-1);
                    }
                }
                else
                {
                    emit signal_login_result(-1);
                    return;
                }
            }
            else
            {
                emit signal_login_result(-1);
                return;
            }

            if(obj.contains("username"))
            {
                QJsonValue nameValue = obj.take("username");
                if(nameValue.isString())
                {
                    name = nameValue.toString();
                }
                else
                {
                    emit signal_login_result(-1);
                    return;
                }
            }
            else
            {
                emit signal_login_result(-1);
                return;
            }

            if(obj.contains("session"))
            {
                QJsonValue sessionValue = obj.take("session");
                if(sessionValue.isString())
                {
                    session = sessionValue.toString();
                }
                else
                {
                    emit signal_login_result(-1);
                    return;
                }
            }
            else
            {
                emit signal_login_result(-1);
                return;
            }
            emit signal_login_result(0);
            emit signal_update_user(name,session);
        }
        else
        {
            emit signal_login_result(-1);
            return;
        }
    }
    else
    {
        emit signal_login_result(-1);
        return;
    }
}

/*******************************************************************************
* Function Name  :  slot_login_failed
* Description    :
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerUser::slot_login_failed()
{
    emit signal_login_result(-1);
}

/*******************************************************************************
* Function Name  :  slot_modify_key
* Description    :
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerUser::slot_user_modify(QString key)
{
    qDebug()<<key;
    QNetworkRequest request;
    NetworkManager *networkManage = new NetworkManager();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    request.setRawHeader("Username", name.toLatin1());
    qDebug()<<session;
    request.setRawHeader("User-Session", session.toLatin1());
    request.setRawHeader("Timestamp", QString::number(QDateTime::currentDateTime().toTime_t()).toLatin1());
    request.setRawHeader("Encrypted", QString::number(QDateTime::currentDateTime().toTime_t()).toLatin1());

#ifdef TEST_MODE
    request.setRawHeader("test-db", "1");
#endif
    QString password;
    QByteArray ba;
    ba = QCryptographicHash::hash (key.toLatin1(), QCryptographicHash::Md5);
    password.append(ba.toHex());

    QString str = QString(URL_SERVER).append(QString(REQUEST_MODIFY));
    QUrl urlModify = QUrl::fromEncoded(str.toLatin1());
    qDebug()<<str;

    QByteArray postData;
    postData.append("new_password=" + password);
    request.setUrl(urlModify);

    connect(networkManage, SIGNAL(signal_request_success(QString)), this, SLOT(slot_modify_success(QString)));
    connect(networkManage, SIGNAL(signal_request_timeout()), this, SLOT(slot_modify_failed()));
    connect(networkManage, SIGNAL(signal_request_failed(QString)), this, SLOT(slot_modify_failed()));
    networkManage->post_request(request,postData);
}

/*******************************************************************************
* Function Name  :  slot_modify_success
* Description    :
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerUser::slot_modify_success(QString replyData)
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
                        emit signal_modify_result(-1);
                        return;
                    }
                    else
                    {
                        emit signal_modify_result(0);
                        return;
                    }
                }
                else
                {
                    emit signal_modify_result(-1);
                    return;
                }
            }
            else
            {
                emit signal_modify_result(-1);
                return;
            }
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_modify_failed
* Description    :
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerUser::slot_modify_failed()
{
    emit signal_modify_result(1);
}

