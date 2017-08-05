#include "widget/widgetMain.h"
#include <QApplication>
#include <QTextCodec>
#include <windows.h>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        #if defined(_MSC_VER) && (_MSC_VER < 1600)
                QTextCodec::setCodecForTr(QTextCodec::codecForName("GB18030-0"));
        #else
                QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
        #endif
   #endif
    QApplication a(argc, argv);
    WidgetMain w;
    w.show();
//    QString data = "43470d54653737420472033656e736f72054572726f720747";
//    data = QString(QByteArray::fromHex(data.toLatin1()));
//    qDebug()<<data;
    return a.exec();
}
