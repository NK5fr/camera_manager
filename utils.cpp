#include "utils.h"

#include <QRect>
#include <QScreen>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qmessagebox.h>
#include <qdatetime.h>
#include <string>
#include <iostream>

// Shorthand to show a messageBox when there's an error
int utils::showError(std::string text)
{
    return QMessageBox(QMessageBox::NoIcon,
                       "Error!",
                       QString::fromStdString(text),
                       QMessageBox::StandardButton::Ok)
        .exec();
}

// Centers a Widget on a given screen
QRect utils::reCenter(QWidget* toCenter, QScreen *screen)
{
    return utils::centerOnPoint(toCenter->geometry(), screen->geometry().center());
}

// Creates and returns a rectangle that is then centered on a given screen
QRect utils::reCenter(int width, int height, QScreen *screen)
{
    QRect rect = QRect(0,0,width, height);
    rect.moveCenter(screen->geometry().center());
    return rect;
}

// Centers a widget on a given screen and then adds an offset to it
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
// Centers a widget to another widget
QRect utils::reCenterWidget(QWidget *toCenter, QWidget *widget)
{
    return utils::centerOnPoint(toCenter->geometry(), widget->mapToGlobal(widget->frameGeometry().center()));
}

// Centers a QRect on to another QRect
QRect utils::center(QRect toCenter, QRect rect)
{
    return utils::centerOnPoint(toCenter, rect.center());
}

// Moves the center of a QRect to a given point, used to center everything
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

std::string utils::getTime()
{
    QDateTime time = QDateTime::currentDateTime();
    std::string stringTime = time.toString().toStdString();
    for(int i = 0; i < stringTime.length(); i++)
    {
        if(stringTime[i] == ' ' | stringTime[i] == ':')
            stringTime[i] = '-';
    }
    qInfo() << stringTime;
    return stringTime;
}
