#ifndef GLOBALDEFINE_H
#define GLOBALDEFINE_H

#define     SYS_VERSION                 "1.0"

#define     URL_SERVER                  "http://manufacture.cleargrass.com"
#define     REQUEST_LOGIN               "/api/login"
#define     REQUEST_MODIFY              "/api/modify_user"
#define     REQUEST_FETCH_MAC           "/api/bran/fetch_mac"
#define     REQUEST_CHECK_FIRMWARE      "/api/bran/check_update"
#define     REQUEST_SYNC_TEST_PASSED            "/api/bran/mac_written"
#define     REQUEST_SYNC_TEST_FAILED            "/api/bran/board_test_failed"

#define     REQUEST_HODOR_TEST_PASS     "/api/bran/test_pass"
#define     REQUEST_HODOR_TEST_FAILED   "/api/bran/test_failed"

#define DEVICE_GRAVITY  "/sys/devices/platform/sun5i-i2c.2/i2c-2/2-0019/get_xyz"
#define DEVICE_CPU      "/proc/cpuinfo"

#define ERROR_DATA 999

#define WIFI_COMMAND1   "./bin/wifi_config.sh /bin/qtapp/"
#define WIFI_COMMAND2   "chmod 777 /bin/qtapp/wifi_config.sh"
#define WIFI_COMMAND3   "/bin/qtapp/wifi_config.sh"
#define WIFI_COMMAND4   "wpa_cli -i wlan0 scan_result"

#define SHELL_DEVICE_NAME       "device_config.sh"
#define SHELL_DEVICE_PATH       QString("./bin/%1").arg(SHELL_DEVICE_NAME)
#define DEVICE_MAC          "mac"
#define DEVICE_DID          "did"
#define DEVICE_KEY          "key"

#define COMMAND_DEVICE_1    QString("%1 /bin/qtapp/").arg(SHELL_DEVICE_PATH)
#define COMMAND_DEVICE_2    QString("chmod 777 /bin/qtapp/%1").arg(SHELL_DEVICE_NAME)
#define COMMAND_DEVICE_3    QString("/bin/qtapp/%1").arg(SHELL_DEVICE_NAME)

enum CommandType
{
    BURN_FIRMWARE_PREPARE = 0,
    BURN_FIRMWARE_START = 1,
    BURN_FIRMWARE_END   = 2,
    CHANGE_USB_A0B0     = 3,
    CHANGE_USB_A0B1     = 4,
    CHANGE_USB_A1B0     = 5,
    CHANGE_USB_A1B1     = 6,
    GET_VOLTAGE         = 7,
    GET_DEVICE          = 8,
    GET_FIXTURE         = 9
};

enum TestStatusType
{
    SUCCESS = 0,
    FAILED  = 1,
    NORMAL  = 2,
    TESTING = 3
};

typedef struct refData
{
    float value;
    float error;
    void init()
    {
        value = ERROR_DATA;
        error = ERROR_DATA;
    }
}RefData;

typedef struct refDatas
{
    float dataVol0;
    float dataVol1;
    float dataVol2;
    float dataVol3;
    float dataVol4;
    float dataVol5;
    float dataVol6;
    float dataVol7;

    float dataError;
    void init()
    {
        dataVol0 = ERROR_DATA;
        dataVol1 = ERROR_DATA;
        dataVol2 = ERROR_DATA;
        dataVol3 = ERROR_DATA;
        dataVol4 = ERROR_DATA;
        dataVol5 = ERROR_DATA;
        dataVol6 = ERROR_DATA;
        dataVol7 = ERROR_DATA;
        dataError = ERROR_DATA;
    }
    void set_error(float error)
    {
        this->dataError = error;
    }
}RefDatas;

typedef struct infoMiio
{
    QString mac;
    QString did;
    QString key;
    void clear()
    {
        mac = "";
        did = "";
        key = "";
    }

    bool isEmpty()
    {
        if((mac == "") || (did == "") || (key == ""))
        {
            return true;
        }
        return false;
    }

}InfoMIIO;

typedef struct infoDevice
{
    QString adb;
    QString did;
    QString mac;
    QString key;
    QString sn;

}InfoDevice;

typedef struct infoHodor
{
    QString sn;
    InfoMIIO infoMIIO;
    bool gravity;
    bool tempHumi;
    bool tvocCo2e;
    bool button;
    bool touch;
    bool wifi;
    bool pm25;
    bool light;
    bool battery;
    infoHodor()
    {
        init();
    }
    void init()
    {
        gravity = false;
        tempHumi = false;
        tvocCo2e = false;
        button = false;
        touch = false;
        wifi = false;
        pm25 = false;
        light = false;
        battery = false;
    }
    bool isPassed()
    {
        if(gravity&&tempHumi&&tvocCo2e&&button&&touch&&wifi&&pm25&&light&&battery)
        {
            return true;
        }
        return false;
    }
}InfoHodor;

typedef struct infoVol
{
    float vol0_LED;
    float vol1_TVOC;
    float vol2_AVCC;
    float vol3_WIFI;
    float vol4_RTC;
    float vol5_IPSOUT;
    float vol6_DRAM;
    float vol7_5V;

    void clear()
    {
        vol0_LED = ERROR_DATA;
        vol1_TVOC = ERROR_DATA;
        vol2_AVCC = ERROR_DATA;
        vol3_WIFI = ERROR_DATA;
        vol4_RTC = ERROR_DATA;
        vol5_IPSOUT = ERROR_DATA;
        vol6_DRAM = ERROR_DATA;
        vol7_5V = ERROR_DATA;
    }

}InfoVol;

typedef struct infoResult
{
    bool cpu;
    bool rtc;
    bool gravity;
    bool wifi;
    bool usb;
    bool vol;
    bool miio;

}InfoResult;

typedef struct infoTest
{
    QString sn;
    QString fixture;
    InfoMIIO infoMIIO;
    InfoVol infoVol;
    InfoResult infoResult;
    infoTest()
    {
        infoMIIO.clear();
        infoVol.clear();
    }
    void clear()
    {
        infoMIIO.clear();
        infoVol.clear();
    }
}InfoTest;

typedef struct wifi
{
    QString name;
    int value;
}Wifi;

#endif // GLOBALDEFINE_H