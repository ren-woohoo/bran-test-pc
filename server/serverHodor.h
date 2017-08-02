#ifndef SERVERHODOR_H
#define SERVERHODOR_H

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

class ServerHodor : public QObject
{
    Q_OBJECT
public:
    explicit ServerHodor(QObject *parent = 0);

signals:
    void signal_syncTest_success(QString,QString);
    void signal_syncTest_failed(QString,QString);


private slots:
    void slot_update_user(QString,QString);

    void slot_sync_testPassed(InfoDevice,InfoHodor);
    void slot_sync_testFailed(InfoDevice,InfoHodor);
    void slot_syncTest_success(QString);
    void slot_syncTest_failed(QString);
    void slot_syncTest_timeout();

private:
    QString session;
    QString name;

    QString requestData;
};

#endif // SERVERHODOR_H
