#include "timewatcher.h"

TimeWatcher* TimeWatcher::timeWatcher()
{
    static TimeWatcher *watcher_=0;
    if (watcher_==0) watcher_=new TimeWatcher;
    return watcher_;
}

TimeWatcher::TimeWatcher(QObject *parent) :
    QObject(parent)
{
    lastTime==QDateTime::currentDateTime();
    this->startTimer(1000);
}


void TimeWatcher::timerEvent(QTimerEvent *)
{
    emit secondChanged();
    QDateTime current=QDateTime::currentDateTime();
    if (current.time().minute()!=lastTime.time().minute()) emit minuteChanged();
    if (current.time().hour()!=lastTime.time().hour()) emit hourChanged();
    if (current.date().day()!=lastTime.date().day()) emit dayChanged();
    if (current.date().weekNumber()!=lastTime.date().weekNumber()) emit weekChanged();
    if (current.date().month()!=lastTime.date().month()) emit monthChanged();
    if (current.date().year()!=lastTime.date().year()) emit yearChanged();

    lastTime=current;
}
