#ifndef TASKMODEL_H
#define TASKMODEL_H

#include <QAbstractListModel>
#include "task.h"

class Task;

class TaskModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TaskModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    inline int rowCount(const QModelIndex &index=QModelIndex()) const
        {Q_UNUSED(index) return tasks.count();}

    inline void sort()
        {qSort(tasks);  emit dataChanged(this->index(0),this->index(tasks.count()-1));}

    inline Qt::ItemFlags flags(const QModelIndex &index=QModelIndex()) const
        {return QAbstractListModel::flags(index);}

    void appendTask(Task task);

    inline QModelIndex indexFromTask(Task task)
        {QModelIndex index=this->index(tasks.indexOf(task));return index;}

    inline Task taskAt(int row) const
    {return tasks.at(row);}

    int uncompleteCount() const;

    QList<Task> getPublicTasks() const;
    QList<Task> getPrivateTasks() const;

    void removeRow(int row, const QModelIndex &parent=QModelIndex());

    inline void clear()
        {this->tasks.clear();emit dataChanged(index(0),index(0));}

    void removeDuplicates();
    void removeCompleted();
    void removePrivate();

    void replaceTask(QModelIndex,Task);

protected:
    QList<Task> tasks;

public slots:
    void setTasks(QList<Task>newTasks);

signals:
    void taskListChanged(QList<Task>);
};

#endif // TASKMODEL_H
