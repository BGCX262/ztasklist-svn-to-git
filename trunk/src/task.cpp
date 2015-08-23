#include <QHash>

#include "task.h"

Task::Task()
{
    this->completed_= false;
    private_=false;
    this->repeat_=Task::NoRepeat;
}

void Task::setPriority(int v)
{
    priority_=v;
}

void Task::setName(QString v)
{
    name_=v;
}

void Task::setDescription(QString v)
{
    description_=v;
}

void Task::setDeadlineTime(QDateTime v)
{
    deadlineTime_=v;
}

void Task::setCreateDate(QDate v)
{
    createDate_=v;
}

void Task::setCompleted(bool v)
{
    completed_=v;
}

void Task::setNotifTime(int v)
{
    notifTime_=v;
}

void Task::setPrivate(bool v)
{
    private_=v;
}

void Task::setRepeatType(RepeatType v)
{
    this->repeat_=v;
}

QString Task::priorityIcon() const
{
    QString priorityImage;
    switch (this->priority_)
    {
        case 0:priorityImage=":/priority/low";break;
        case 1:priorityImage=":/priority/normal";break;
        case 2:priorityImage=":/priority/high";break;
    }
    return priorityImage;
}

//Operators
bool Task::operator<(const Task task) const
{
    if (task.completed_) return true;
    if (this->completed_) return false;

    if (!task.deadlineTime_.isValid())
    {
        if (deadlineTime_.isValid()) return true;
        else
        {
            if (task.priority_==priority_)
                return task.name_<this->name_;
            else
            return  task.priority_<priority_;
        }
    }
    else
        if (!deadlineTime_.isValid())
        {
            if (task.deadlineTime_.isValid()) return false;
            else
                if (task.priority_==priority_)
                    return task.name_<this->name_;
                else
                return  task.priority_<priority_;
        } else
        if (task.deadlineTime_==deadlineTime_)
        {
            if (task.priority_==priority_)
                return task.name_<this->name_;
            else
            return task.priority_<priority_;
        }
        else
            return task.deadlineTime_>deadlineTime_;
}

void Task::operator=(Task t)
{
    this->name_=t.name_;
    this->description_=t.description_;
    this->completed_=t.completed_;
    this->deadlineTime_=t.deadlineTime_;
    this->notifTime_=t.notifTime_;
    this->priority_=t.priority_;
    this->private_=t.private_;
    this->createDate_=t.createDate_;
    this->repeat_=t.repeat_;
}

bool Task::operator==(Task task) const
{
    return (this->completed_==task.completed_ &&
            this->name_==task.name_ &&
            this->description_==task.description_ &&
            this->deadlineTime_==task.deadlineTime_ &&
            this->priority_==task.priority_ &&
            this->notifTime_==task.notifTime_ &&
            this->private_==task.private_ &&
            this->createDate_==task.createDate_ &&
            this->repeat_==task.repeat_);
}

bool Task::operator!=(Task task) const
{
    return !(*this==task);
}

QDataStream &operator<<(QDataStream &stream, const Task &task)
{
    stream<<task.name();
    stream<<task.description();
    stream<<task.priority();
    stream<<task.notifTime();
    stream<<task.deadlineTime().toString(Qt::ISODate);
    stream<<task.createDate().toString(Qt::ISODate);
    stream<<task.completed();
    stream<<(int)task.repeatType();
    stream<<task.isPrivate();
    return stream;
}

QDataStream &operator>>(QDataStream &stream, Task &task)
{
    QString tempS;
    int tempI;
    bool tempB;

    stream>>tempS; task.setName(tempS);
    stream>>tempS; task.setDescription(tempS);
    stream>>tempI; task.setPriority(tempI);
    stream>>tempI; task.setNotifTime(tempI);
    stream>>tempS; task.setDeadlineTime(QDateTime::fromString(tempS,Qt::ISODate));
    stream>>tempS; task.setCreateDate(QDate::fromString(tempS,Qt::ISODate));
    stream>>tempB; task.setCompleted(tempB);
    stream>>tempI; task.setRepeatType((Task::RepeatType)tempI);
    stream>>tempB; task.setPrivate(tempB);
    return stream;
}

uint qHash(const Task &task)
{
   return  qHash(task.name()+"|"+
           task.description()+"|"+
           task.priority()+"|"+
           task.deadlineTime().toString()+"|"+
           task.completed()+"|"+
           task.notifTime()+"|"+
           task.isPrivate()+"|"+
           task.createDate().toString());
}
