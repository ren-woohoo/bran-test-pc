#ifndef FILECONFIG_H
#define FILECONFIG_H

#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QTextCodec>
#include "globaldefine.h"

#define SYSTEM_CONFIG_PATH  "./data.conf"

#define DATA_ERROR  "DATA_ERROR"
#define REF_LED     "REF_LED"
#define REF_TVOC    "REF_TVOC"
#define REF_AVCC    "REF_AVCC"
#define REF_WIFI    "REF_WIFI"
#define REF_RTC     "REF_RTC"
#define REF_IPSOUT  "REF_IPSOUT"
#define REF_DRAM    "REF_DRAM"
#define REF_5V      "REF_5V"

#define DEFAULT_DATA_ERROR      "0.5"
#define DEFAULT_REF_LED         "5"
#define DEFAULT_REF_TVOC        "1.8"
#define DEFAULT_REF_AVCC        "3"
#define DEFAULT_REF_WIFI        "3.3"
#define DEFAULT_REF_RTC         "3.3"
#define DEFAULT_REF_IPSOUT      "5"
#define DEFAULT_REF_DRAM        "1.5"
#define DEFAULT_REF_5V          "5"

typedef struct SystemValue
{
    QString name;
    QString value;

    SystemValue(QString name,QString value)
    {
        this->name = name;
        this->value = value;
    }

}SystemValue;

class FileConfig : public QObject
{
    Q_OBJECT
public:
    static FileConfig *getInstance();
    QString get_value(QString);
    void set_value(QString name,QString value);

signals:

public slots:

private:
    void list_init(void);                                               // 初始化全局配置
    void config_init();
    void config_save();

private:
    QList<SystemValue> globalList;

private:
    explicit FileConfig(QObject *parent = 0);
    static FileConfig *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(FileConfig::instance)
                delete FileConfig::instance;
        }
    };
    static FreeInstance free;

};

#endif // FILECONFIG_H
