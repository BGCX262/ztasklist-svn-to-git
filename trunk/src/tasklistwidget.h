#ifndef TASKLISTWIDGET_H
#define TASKLISTWIDGET_H

#include <QListWidget>

#include "taskfiltermodel.h"
#include "task.h"

class TaskModel;

class TaskListWidget : public QListView
{
Q_OBJECT
public:
    explicit TaskListWidget(QWidget *parent = 0);
    ~TaskListWidget();
    void appendTask(Task newTask);
    QList<Task> publicTasks();
    QList<Task> privateTasks();
    void clear();
    int count();
    int uncompleteCount();
    void removeCompleted();
    void removeDuplicates();

    Task task(int);
    void replaceTask(QModelIndex,Task);
    void setPriority(QModelIndex,int);
    void setRepeat(QModelIndex,Task::RepeatType);

    void setSourceModel(TaskModel *model);

public slots:
    void toggleTask(QModelIndex);
    void toggleTask(int);
    void toggleTask(Task);
    void togglePrivateTask(QModelIndex);
    void removeTask(QModelIndex);
    void setCompleteHidden(bool);
    void setOnlyTodayShow(bool v);
    void setPrivacyFilterState(TaskFilterModel::privacyFilterMode);
    void setNameFilter(QString);
    void setTasks(QList<Task>,bool clear=true);

private slots:
    void currentChanged(QModelIndex,QModelIndex);

private:
    TaskModel *taskListModel;
    TaskFilterModel *filterModel;
    QPoint startDragPos;

    void startDrag();
    TaskModel *model() const;
    QString timeFromNow(QDateTime);


protected:
    virtual bool event(QEvent *e);
    virtual void paintEvent(QPaintEvent *e);
    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void dragEnterEvent(QDragEnterEvent *);
    virtual void dragMoveEvent(QDragMoveEvent *e);
    virtual void dropEvent(QDropEvent *e);
    virtual bool viewportEvent(QEvent *event);

signals:
    void mouseEntered();
    void mouseLeave();
    void selectedItemChanged(QModelIndex);
    void taskListChanged(QList<Task>);
};

#endif // TASKLISTWIDGET_H
