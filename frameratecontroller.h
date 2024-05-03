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

private:
    Ui::FrameRateController *ui;
};

#endif // FRAMERATECONTROLLER_H
