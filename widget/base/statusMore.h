#ifndef STATUSMORE_H
#define STATUSMORE_H

#include <QWidget>

namespace Ui {
class StatusMore;
}

class StatusMore : public QWidget
{
    Q_OBJECT

public:
    explicit StatusMore(QWidget *parent = 0);
    ~StatusMore();
    void setEnabled(bool);

signals:
    void clicked();


private slots:
    void on_button_more_clicked();

private:
    Ui::StatusMore *ui;
};

#endif // STATUSMORE_H
