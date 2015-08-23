#include <QSet>
#include "taskmodel.h"

TaskModel::TaskModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

QVariant TaskModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    Task current=tasks.at(index.row());

    switch(role)
    {
        case Task::NameRole:return QVariant(current.name());break;
        case Task::DescriptionRole:return QVariant(current.description());  break;
        case Task::PriorityRole:return QVariant(current.priority());break;
        case Task::DeadlineRole:return QVariant(current.deadlineTime());break;
        case Task::CompleteStatusRole:return QVariant((int)current.completed());break;
        case Task::NotifTimeRole:return QVariant(current.notifTime());break;
        case Task::PrivateRole:return QVariant(current.isPrivate());break;
        case Task::CreatedRole:return QVariant(current.createDate());break;
        case Task::RepeatRole:return QVariant((int)current.repeatType());break;
    }
    return QVariant();
}

bool TaskModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) return false;
    int row=index.row();

    switch(role)
    {
        case Task::NameRole:tasks[row].setName(value.toString()); break;
        case Task::DescriptionRole:tasks[row].setDescription(value.toString()); break;
        case Task::PriorityRole:tasks[row].setPriority(value.toInt()); break;
        case Task::DeadlineRole:tasks[row].setDeadlineTime(value.toDateTime()) ;break;
        case Task::CompleteStatusRole:tasks[row].setCompleted(value.toBool()) ;break;
        case Task::NotifTimeRole:tasks[row].setNotifTime(value.toInt()) ;break;
        case Task::PrivateRole:tasks[row].setPrivate(value.toBool()) ;break;
        case Task::CreatedRole:return false;
        case Task::RepeatRole:tasks[row].setRepeatType((Task::RepeatType)value.toInt());break;
        default:return false;
    }
    sort();
    emit taskListChanged(tasks);
    return true;
}

QList<Task> TaskModel::getPublicTasks() const
{
    QList<Task> result;
    foreach (Task task,tasks)
    {
        if (!task.isPrivate())result.append(task);
    }
    return result;
}

QList<Task> TaskModel::getPrivateTasks() const
{
    QList<Task> result;
    foreach (Task task,tasks)
    {
        if (task.isPrivate())result<<task;
    }
    return result;
}

void TaskModel::removeDuplicates()
{
    QSet<Task> temp=tasks.toSet();
    tasks=temp.toList();
    sort();
    emit taskListChanged(tasks);
}

void TaskModel::removeCompleted()
{
    for (int i=tasks.count()-1;i>=0;--i)
    {
        if (tasks[i].completed() && tasks[i].repeatType()==Task::NoRepeat) tasks.removeAt(i);
    }
    sort();
    emit taskListChanged(tasks);
}

int TaskModel::uncompleteCount() const
{
    int result=0;
    foreach(Task task,tasks)
    {
        if (!task.completed()) result++;
    }
    return result;
}

void TaskModel::appendTask(Task task)
{
    tasks.append(task); sort();
    emit taskListChanged(tasks);
}

void TaskModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    tasks.removeAt(row);
    sort();
    emit taskListChanged(tasks);
}

void TaskModel::setTasks(QList<Task>newTasks)
{
    tasks=newTasks;
    sort();
}

void TaskModel::replaceTask(QModelIndex index,Task replace)
{
    tasks.removeAt(index.row());
    tasks.append(replace);
    sort();
    emit taskListChanged(tasks);
}
