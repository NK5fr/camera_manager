#ifndef FRAMERATECONTROLLER_H
#define FRAMERATECONTROLLER_H

#include <QWidget>

namespace Ui {
class FrameRateController;
}

class FrameRateController : public QWidget
{
    Q_OBJECT

public:
    explicit FrameRateController(QWidget *parent = nullptr);
    ~FrameRateController();
signals:
    void fixedFrameRateChanged(int);
private:
    Ui::FrameRateController *ui;
private slots:
    void changeFixedFrameRate(int value);
};

#endif // FRAMERATECONTROLLER_H
