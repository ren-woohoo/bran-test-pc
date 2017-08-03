#ifndef SERVERMAC_H
#define SERVERMAC_H
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkInterface>
#include <QNetworkRequest>
#include <QUrl>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "networkManager.h"
#include "globaldefine.h"

class ServerMIIO : public QObject
{
    Q_OBJECT
public:
    explicit ServerMIIO(QObject *parent = 0);

signals:
    void signal_fetch_success(QString, InfoMIIO);
    void signal_fetch_failed(QString, QString);

private slots:
    void slot_update_user(QString,QString);

    void slot_fetch_mac(QString);
    void slot_fetch_success(QString);
    void slot_fetch_failed(QString);
    void slot_fetch_timeout();

private:
    QString session;
    QString name;
    QString requestData;

};

#endif // SERVERMAC_H
