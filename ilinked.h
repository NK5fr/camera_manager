#ifndef ILINKED_H
#define ILINKED_H

#include <QObject>
#include <QLabel>

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
