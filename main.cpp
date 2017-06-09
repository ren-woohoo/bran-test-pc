#include "widgetMain.h"
#include <QApplication>
#include <QTextCodec>
#include "widget/base/widgetDebugInfo.h"

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
    QString str ="\xE9\x9D\x9E\xE6\xB3\x95\xE8\xAF\xB7\xE6\xB1\x82\xEF\xBC\x8C\xE5\x8A\xA0\xE5\xAF\x86\xE4\xBF\xA1\xE6\x81\xAF\xE6\x9C\x89\xE9\x94\x99";
    qDebug()<<str;
//    WidgetDebugInfo w;
    w.show();
    return a.exec();
}
