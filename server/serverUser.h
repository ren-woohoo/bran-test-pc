#ifndef SERVERUSER_H
#define SERVERUSER_H

#include <QNetworkRequest>
#include <QUrl>
#include <QDateTime>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "networkManager.h"
#include "globaldefine.h"

class ServerUser : public QObject
{
    Q_OBJECT
public:
    static ServerUser *getInstance();

signals:
    void signal_login_result(int);
    void signal_modify_result(int);
    void signal_update_user(QString,QString);

private slots:
    void slot_user_login(QString,QString);
    void slot_user_modify(QString);
    void slot_login_failed();
    void slot_modify_failed();
    void slot_login_success(QString);
    void slot_modify_success(QString);

private:
    QString session;
    QString name;

private:
    explicit ServerUser(QObject *parent = 0);
    static ServerUser *instance;

};

#endif // SERVERUSER_H
