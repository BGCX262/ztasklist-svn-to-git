#ifndef TASK_H
#define TASK_H

#include <QDateTime>

class QString;

class Task
{
public:
    enum RepeatType{NoRepeat,DayRepeat,WeekRepeat,monthRepeat};
    enum TaskRole
    {
        NameRole=Qt::UserRole,
        DescriptionRole=Qt::ToolTipRole,
        PriorityRole=Qt::UserRole+1,
        DeadlineRole=Qt::UserRole+2,
        CompleteStatusRole=Qt::UserRole+3,
        NotifTimeRole=Qt::UserRole+4,
        PrivateRole=Qt::UserRole+5,
        CreatedRole=Qt::UserRole+6,
        RepeatRole=Qt::UserRole+7
    };

    Task();
    inline int priority() const
        {return priority_;}
    void setPriority(int);

    inline QString name()const
        {return name_;}
    void setName(QString);

    inline QString description() const
        {return description_;}
    void setDescription(QString);

    inline QDateTime deadlineTime() const
        {return deadlineTime_;}
    void setDeadlineTime(QDateTime);

    inline QDate createDate() const
     {return createDate_;}
    void setCreateDate(QDate);

    inline bool completed() const
        {return completed_;}
    void setCompleted(bool);

    inline int notifTime() const
        {return notifTime_;}
    void setNotifTime(int);

    inline bool isPrivate() const
        {return private_;}
    void setPrivate(bool);

    inline RepeatType repeatType() const
        {return repeat_;}
    void setRepeatType(RepeatType);

    inline bool isActual() const
        {return deadlineTime_>QDateTime::currentDateTime();}
    QString priorityIcon() const;

    bool operator==(Task) const;
    bool operator!=(Task) const;
    bool operator<(const Task) const;
    void operator=(Task);

private:
    int priority_;
    int notifTime_;
    QString name_;
    QString description_;
    QDateTime deadlineTime_;
    QDate createDate_;
    bool completed_;
    RepeatType repeat_;
    bool private_;
};

uint qHash(const Task &task);

QDataStream &operator<<(QDataStream &, const Task &);
QDataStream &operator>>(QDataStream &, Task &);

#endif // TASK_H
