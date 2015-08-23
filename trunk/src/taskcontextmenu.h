#ifndef TASKCONTEXTMENU_H
#define TASKCONTEXTMENU_H

#include <QMenu>
#include "task.h"

class TaskContextMenu : public QMenu
{
    Q_OBJECT
public:
    explicit TaskContextMenu(QWidget *parent = 0);

signals:
    void toggleComplete();
    void togglePrivate();
    void removeTask();
    void priorityChanged(int);
    void repeatChaged(Task::RepeatType);


private:
    QAction *actionHigh;
    QAction *actionNormal;
    QAction *actionLow;
    QAction *actionComplete;
    QAction *actionRemove;
    QAction *actionPrivate;

    QAction *actionNoRepeat;
    QAction *actionDayly;
    QAction *actionWeekly;
    QAction *actionmonthly;

private slots:
    void prioritySelected(QAction*);

};

#endif // TASKCONTEXTMENU_H
