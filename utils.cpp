#include "utils.h"

#include <QRect>
#include <QScreen>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qmessagebox.h>

QRect utils::reCenter(QRect rect, QScreen *screen)
{
    QRect newRect = QRect(0,0, rect.width(), rect.height());
    newRect.moveCenter(screen->geometry().center());
    return newRect;
}

QRect utils::reCenter(QScreen *screen, int width, int height) {
    QRect newRect = QRect(0,0, width, height);
    newRect.moveCenter(screen->geometry().center());
    return newRect;
}

QRect utils::reCenterOffSet(QRect rect, QScreen *screen, char direction, int offset)
{
    QRect newRect = QRect(0,0, rect.width(), rect.height());
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
    newRect.moveCenter(screenCenter);
    return newRect;
}

int utils::showError(std::string text)
{
    return QMessageBox(QMessageBox::NoIcon, "Error!", QString::fromStdString(text), QMessageBox::StandardButton::Ok).exec();

}
