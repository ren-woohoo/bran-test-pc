#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QEventLoop>
#include <QDebug>
#include <QFile>
#include <QTextCodec>
#include <QCryptographicHash>
#include <QDir>
#include "globaldefine.h"

class NetworkManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    NetworkManager();
    void get_request(QNetworkRequest);
    void post_request(QNetworkRequest,QByteArray);
    QString encrypted_string(QString);

signals:
    void signal_request_success(QString);
    void signal_request_failed(QString);
    void signal_request_timeout();

protected slots:


private:
    int durationRequest;
};

#endif // NETWORKMANAGER_H
