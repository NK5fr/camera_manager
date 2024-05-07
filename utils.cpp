#include "utils.h"

#include <QRect>
#include <QScreen>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qmessagebox.h>

int utils::showError(std::string text)
{
    return QMessageBox(QMessageBox::NoIcon,
                       "Error!",
                       QString::fromStdString(text),
                       QMessageBox::StandardButton::Ok)
        .exec();
}

QRect utils::reCenter(QWidget* toCenter, QScreen *screen)
{
    return utils::centerOnPoint(toCenter->geometry(), screen->geometry().center());
}

QRect utils::reCenter(int width, int height, QScreen *screen)
{
    QRect rect = QRect(0,0,width, height);
    rect.moveCenter(screen->geometry().center());
    return rect;
}

QRect utils::reCenterOffSet(QWidget *toCenter, QScreen *screen, char direction, int offset)
{
    QPoint screenCenter = screen->geometry().center();
    switch (direction) {
    case 'b':
        screenCenter.setY(screenCenter.y() + offset);
        break;
    case 'r':
        screenCenter.setX(screenCenter.x() + offset);
        break;
    case 'l':
        screenCenter.setX(screenCenter.x() - offset);
        break;
    default:
        screenCenter.setY(screenCenter.y() - offset);
        break;
    }
    return utils::centerOnPoint(toCenter->geometry(), screenCenter);
}

QRect utils::reCenterWidget(QWidget *toCenter, QWidget *widget)
{
    return utils::centerOnPoint(toCenter->geometry(), widget->mapToGlobal(widget->frameGeometry().center()));
}

QRect utils::center(QRect toCenter, QRect rect)
{
    return utils::centerOnPoint(toCenter, rect.center());
}

QRect utils::centerOnPoint(QRect toCenter, QPoint point)
{
    toCenter.moveCenter(point);
    return toCenter;
}

QRect utils::offSet(QWidget *toOffset, char direction, int offset)
{
    QPoint offsetPoint = toOffset->geometry().center();
    switch (direction) {
    case 'b':
        offsetPoint.setY(offsetPoint.y() + offset);
        break;
    case 'r':
        offsetPoint.setX(offsetPoint.x() + offset);
        break;
    case 'l':
        offsetPoint.setX(offsetPoint.x() - offset);
        break;
    default:
        offsetPoint.setY(offsetPoint.y() - offset);
        break;
    }
    return utils::centerOnPoint(toOffset->geometry(), offsetPoint);
}
