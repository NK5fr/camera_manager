#include "utils.h"

#include <QRect>
#include <QScreen>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qmessagebox.h>

QRect utils::reCenter(int width, int height, QScreen *screen) {
    QRect newRect = QRect(0,0, width, height);
    newRect.moveCenter(screen->geometry().center());
    return newRect;
}

void  utils::reCenterOffSet(QWidget toCenter, QScreen *screen, char direction, int offset)
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
    utils::centerOnPoint(toCenter.geometry(), screenCenter);
}

int utils::showError(std::string text)
{
    return QMessageBox(QMessageBox::NoIcon, "Error!", QString::fromStdString(text), QMessageBox::StandardButton::Ok).exec();

}


void utils::center(QRect toCenter, QRect rect)
{
    toCenter.moveCenter(rect.center());
}

void utils::reCenterWidget(QWidget toCenter, QWidget widget)
{
    utils::center(toCenter.geometry(), widget.geometry());
}

void utils::reCenter(QWidget toCenter, QScreen *screen)
{
    utils::center(toCenter.geometry(), screen->geometry());
}

void utils::centerOnPoint(QRect toCenter, QPoint point)
{
    toCenter.moveCenter(point);
}
