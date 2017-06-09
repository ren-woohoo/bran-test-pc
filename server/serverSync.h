#ifndef SERVERSYNC_H
#define SERVERSYNC_H

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

class ServerSync : public QObject
{
    Q_OBJECT
public:
    explicit ServerSync(QObject *parent = 0);

signals:
    void signal_syncTest_success();
    void signal_syncTest_failed(QString);


private slots:
    void slot_update_user(QString,QString);

    void slot_sync_testPassed(InfoTest);
    void slot_sync_testFailed(InfoTest);
    void slot_syncTest_success(QString);
    void slot_syncTest_failed(QString);
    void slot_syncTest_timeout();

private:
    QString session;
    QString name;

};

#endif // SERVERSYNC_H
