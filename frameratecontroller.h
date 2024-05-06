#ifndef FRAMERATECONTROLLER_H
#define FRAMERATECONTROLLER_H

#include <QWidget>

#include <iostream>
#include <sstream>


using namespace std;

namespace Ui {
class FrameRateController;
}

class FrameRateController : public QWidget
{
    Q_OBJECT

public:
    explicit FrameRateController(QWidget *parent = nullptr);
    Ui::FrameRateController *ui;
    ~FrameRateController();
signals:
    void fixedFrameRateChanged(int);
private:
private slots:
    void changeFixedFrameRate(int value);
};

#endif // FRAMERATECONTROLLER_H
