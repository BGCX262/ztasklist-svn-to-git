#include "taskfiltermodel.h"
#include "taskmodel.h"

TaskFilterModel::TaskFilterModel(QObject * parent): QSortFilterProxyModel(parent)
{
    this->completedHidden=true;
    nonTodayHide_=false;
    filterState_=all;
}

bool TaskFilterModel::completedHide()
{
    return completedHidden;
}

void TaskFilterModel::setCompletedHide(bool v)
{
    completedHidden=v;
    this->invalidate();
}

void TaskFilterModel::setNonTodayHide(bool v)
{
    nonTodayHide_=v;
    this->invalidate();
}

void TaskFilterModel::setSourceModel(TaskModel *sourceModel)
{
    connect(sourceModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(invalidate()));
    connect(sourceModel,SIGNAL(taskListChanged(QList<Task>)),this,SIGNAL(taskListChanged(QList<Task>)));
    QSortFilterProxyModel::setSourceModel(sourceModel);
}

void TaskFilterModel::setPrivacyFilterMode(privacyFilterMode v)
{
    filterState_=v;
    this->invalidate();
}

void TaskFilterModel::setNameFilter(QString v)
{
    filterString_=v;
    this->invalidate();
}

bool TaskFilterModel::filterAcceptsRow(int row, const QModelIndex &) const
{
    Task current=sourceModel()->taskAt(row);
    if (completedHidden && current.completed()%2==1)  return false;
    if (nonTodayHide_ && current.deadlineTime().date()!=QDateTime::currentDateTime().date() && current.deadlineTime().isValid()
        && current.isActual()) return false;

    if (filterState_!=all)
    {
    switch (filterState_)
    {
        case all:break;
        case onlyPublic:
            if (current.isPrivate()) return false; break;
        case onlyPrivate:
            if (!current.isPrivate()) return false;break;
        }
    }

    if (!current.name().toLower().contains(filterString_.toLower())) return false;

    return true;
}

TaskModel *TaskFilterModel::sourceModel() const
{
    return qobject_cast<TaskModel*>(QSortFilterProxyModel::sourceModel());
}
