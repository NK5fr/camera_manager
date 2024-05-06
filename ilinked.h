#ifndef ILINKED_H
#define ILINKED_H

#include <QLabel>
#include <QObject>

class ILinked : public QObject
{
    Q_OBJECT
public:
    virtual ~ILinked() {}
    QLabel *label;
    QString title;
public slots:
    virtual void changeLabel() {}
};
#endif // ILINKED_H
