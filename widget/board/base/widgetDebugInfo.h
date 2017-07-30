#ifndef WIDGETDEBUGINFO_H
#define WIDGETDEBUGINFO_H

#include <QWidget>
#include <QPoint>
#include <QDebug>
#include <QMouseEvent>
#include <QDateTime>
#include <QDir>

namespace Ui {
class WidgetDebugInfo;
}

class WidgetDebugInfo : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetDebugInfo(QWidget *parent = 0);
    ~WidgetDebugInfo();
    void set_debugInfo(QString);
    void set_debugTitle(QString);

protected:
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);

private slots:
    void on_button_save_clicked();

    void on_textBrowser_debugInfo_textChanged();

    void on_button_close_clicked();

private:
    Ui::WidgetDebugInfo *ui;
    QPoint dragPosition;
    QString debugInfo;
    QString debugTitle;
};

#endif // WIDGETDEBUGINFO_H
