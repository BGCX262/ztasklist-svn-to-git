#include <QDomDocument>
#include <QFile>

#include "trackingtaskmodel.h"
#include "timewatcher.h"
#include "datamanager.h"

#include "programsettings.h"

TrackingTaskModel *TrackingTaskModel::getSharedModel()
{
    static TrackingTaskModel *model=0;;
    if (model==0)
    {
        model=new TrackingTaskModel();
    }
    return model;
}

TrackingTaskModel::TrackingTaskModel(QObject *parent):
TaskModel(parent)
{
    this->setTasks(DataManager::dataManager()->tasks());

    connect(DataManager::dataManager(),SIGNAL(taskListChanged(QList<Task>)),this,SLOT(setTasks(QList<Task>)));
    connect(this,SIGNAL(taskListChanged(QList<Task>)),DataManager::dataManager(),SLOT(setTasks(QList<Task>)));

    lastBackupPath_=ProgramSettings::settings()->backupPath();

    timeWatcher=TimeWatcher::timeWatcher();
    connect(timeWatcher,SIGNAL(minuteChanged()),this,SLOT(checkDeadlines()));
}

void TrackingTaskModel::refresh()
{
    DataManager::dataManager()->refresh();
}

inline int immortalDuration()
{
    if (ProgramSettings::settings()->immortalNotifEnabled()) return 0;
        else return ProgramSettings::settings()->notifDuration();
}

void TrackingTaskModel::checkDeadlines()
{
    foreach (Task task, this->tasks)
    {
        if (task.completed() && task.repeatType()==0) return;
        QDateTime current;
        current.setDate(QDate::currentDate());
        QTime time(QTime::currentTime().hour(),QTime::currentTime().minute(),0);
        current.setTime(time);

        if (current.addSecs(ProgramSettings::settings()->notifTime()*60)==task.deadlineTime())
            ProgramSettings::settings()->showNotify(tr("Attention!"),tr("Task <b>\"%1\"</b> must be completed!").arg(task.name()),immortalDuration(),task.priorityIcon());

        if (task.notifTime()>0)
        if (current.addSecs(task.notifTime()*60)==task.deadlineTime())
            ProgramSettings::settings()->showNotify(tr("Attention!"),tr("Task <b>\"%1\"</b> must be completed in %2 minutes!").arg(task.name()).arg(task.notifTime()),immortalDuration(),task.priorityIcon());
        if (current==task.deadlineTime())
            ProgramSettings::settings()->showNotify(tr("Attention!"),tr("Deadline for task <b>\"%1\"</b> now!").arg(task.name()),immortalDuration(),task.priorityIcon());

        if (task.repeatType()>0 && task.completed())
        {
            if (task.createDate()==QDate::currentDate()) return;
            QDate created=task.createDate();
            switch (task.repeatType())
            {
                case Task::NoRepeat: return; break;
                case Task::DayRepeat: created=created.addDays(1); break;
                case Task::WeekRepeat: created=created.addDays(7); break;
                case Task::monthRepeat:created=created.addMonths(1); break;
            }
            if (created==QDate::currentDate())
            {
                QModelIndex index=indexFromTask(task);
                this->setData(index,true,Task::CompleteStatusRole);
                ProgramSettings::settings()->showNotify(tr("Repeatable task toggled!"),tr("Repeatable task <b>\"%1\"</b> is active now!").arg(task.name()),0,":/icons/clock");
            }
        }
    }
}
