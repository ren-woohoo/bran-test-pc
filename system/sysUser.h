#ifndef SYSUSER_H
#define SYSUSER_H
#include <QThread>
#include "server/serverUser.h"

class SysUser : public QThread
{
    Q_OBJECT
public:
    static SysUser* getInstance();

signals:
    void signal_user_login(QString,QString);
    void signal_user_modify(QString);
    void signal_login_result(int);
    void signal_modify_result(int);

private:
    void data_init();
    void connect_init();

private:
    ServerUser *serverUser;

private:
    SysUser();
    static SysUser* instance;

};

#endif // SYSUSER_H
