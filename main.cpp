#include "widget/widgetMain.h"
#include <QApplication>
#include <QTextCodec>
#include <windows.h>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

/*******************************************************************************
* Function Name :   output_msg
* Description   :   自定义log输出的格式
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void output_msg(QtMsgType type, const QMessageLogContext& context, const QString &msg)
{
    static QMutex mutex;

    switch(type)
    {
    case QtDebugMsg:
        fprintf(stderr, "%s\n", msg);
        break;
    case QtWarningMsg:
        fprintf(stderr, "%s\n", msg);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "%s\n", msg);
        break;
    case QtFatalMsg:
        fprintf(stderr, "%s\n", msg);
        abort();
    }
    mutex.lock();
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss:zzz");
    QString thread = QString::number(quintptr(QThread::currentThreadId()));
    QString desc = msg;
    QString log = QString("[%1][%2-%3-%4-%5](%6)").arg(time).arg(context.file).arg(context.function).arg(context.line).arg(thread).arg(desc);
    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString logPath = QString("./%1.log").arg(date);
    QFile file(logPath);
    if(file.open(QIODevice::WriteOnly|QIODevice::Append) && (file.size() < (1024*1024*2)))
    {
        QTextStream textStream(&file);
        textStream.setCodec(QTextCodec::codecForName("UTF-8"));
        textStream<<log<<"\r\n";
        file.close();
    }
    mutex.unlock();
}

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
//    qInstallMessageHandler(output_msg);
    WidgetMain w;
    w.show();
//    QString data = "43470d54653737420472033656e736f72054572726f720747";
//    data = QString(QByteArray::fromHex(data.toLatin1()));
//    qDebug()<<data;
    return a.exec();
}
