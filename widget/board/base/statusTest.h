#ifndef STATUSTEST_H
#define STATUSTEST_H

#include <QWidget>
#include "globaldefine.h"

namespace Ui {
class StatusTest;
}

class StatusTest : public QWidget
{
    Q_OBJECT

public:
    explicit StatusTest(QWidget *parent = 0);
    ~StatusTest();
    void set_status(TestStatusType);

private:
    Ui::StatusTest *ui;
};

#endif // STATUSTEST_H
