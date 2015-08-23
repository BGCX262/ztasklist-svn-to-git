#ifndef TIMEWATCHER_H
#define TIMEWATCHER_H

#include <QObject>
#include <QDateTime>

class TimeWatcher : public QObject
{
    Q_OBJECT
public:
    static TimeWatcher* timeWatcher();

private:
    QDateTime lastTime;
    explicit TimeWatcher(QObject *parent = 0);

protected:
    void timerEvent(QTimerEvent *);

signals:
    void secondChanged();
    void minuteChanged();
    void hourChanged();
    void dayChanged();
    void weekChanged();
    void monthChanged();
    void yearChanged();

};

#endif // TIMEWATCHER_H
